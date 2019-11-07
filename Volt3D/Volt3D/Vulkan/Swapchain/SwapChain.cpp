﻿/**
*	@file Swapchain.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Swapchain.h"

#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "Vulkan/Context.h"
#include "Vulkan/Devices/LogicalDevice.h"
#include "Vulkan/Devices/PhysicalDevice.h"
#include "Vulkan/Images/DepthImage.h"

V3D_NS_BEGIN
VK_NS_BEGIN

v3d::vulkan::Swapchain::Swapchain()
	: swapchain( nullptr )
	, surfaceFormat()
	, extent()
	, images()
	, imageViews()
	, depthImage( nullptr )
{}

Swapchain::~Swapchain()
{
	auto& logicalDevice = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice();
	for (auto& imageView : imageViews) { logicalDevice.destroyImageView( imageView ); }
	imageViews.clear();
	logicalDevice.destroySwapchainKHR( swapchain );
	images.clear();

	SAFE_DELETE( depthImage );
}

bool Swapchain::init()
{
	auto& logicalDevice = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice();
	auto& physicalDevice = v3d::vulkan::PhysicalDevice::get()->getVKPhysicalDevice();
	auto& surface = v3d::vulkan::Context::get()->getSurface();
	auto window = v3d::Engine::get()->getWindow();

	// select format
	std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevice.getSurfaceFormatsKHR( surface );
	if (surfaceFormats.empty()) return false;
	surfaceFormat = selectSurfaceFormat( surfaceFormats );

	// select extent
	const vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR( surface );
	extent = selectExtent( surfaceCapabilities, window->getFrameBufferSize() );

	// select present mode
	std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR( surface );
	if (presentModes.empty()) return false;
	const vk::PresentModeKHR swapchainPresentMode = selectPresentMode( presentModes );

	vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform;

	vk::CompositeAlphaFlagBitsKHR compositeAlpha =
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit : vk::CompositeAlphaFlagBitsKHR::eOpaque;

	vk::SwapchainCreateInfoKHR createInfo
	(
		vk::SwapchainCreateFlagsKHR(),
		surface,
		surfaceCapabilities.minImageCount,
		surfaceFormat.format,
		surfaceFormat.colorSpace,
		extent,
		1,
		vk::ImageUsageFlagBits::eColorAttachment,
		vk::SharingMode::eExclusive,
		0,
		nullptr,
		preTransform,
		compositeAlpha,
		swapchainPresentMode,
		true,
		nullptr
	);

	/*
	uint32_t queueFamilyIndices[2] = { physicalDevice.getGraphicsQueueFamilyIndex(), physicalDevice.getPresentQueueFamilyIndex() };
	if( queueFamilyIndices[0] != queueFamilyIndices[1] )
	{
		// If the graphics and present queues are from different queue families, we either have to explicitly transfer ownership of images between
		// the queues, or we have to create the swapchain with imageSharingMode as VK_SHARING_MODE_CONCURRENT
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	*/

	swapchain = logicalDevice.createSwapchainKHR( createInfo );

	images = logicalDevice.getSwapchainImagesKHR( swapchain );

	imageViews.reserve( images.size() );
	vk::ComponentMapping componentMapping( vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA );
	vk::ImageSubresourceRange subResourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 );

	for (auto& image : images)
	{
		vk::ImageViewCreateInfo createInfo
		(
			vk::ImageViewCreateFlags(),
			image,
			vk::ImageViewType::e2D,
			surfaceFormat.format,
			componentMapping,
			subResourceRange
		);

		imageViews.push_back( logicalDevice.createImageView( createInfo, nullptr ) );
	}

	depthImage = new v3d::vulkan::DepthImage();

	const std::vector<vk::Format> depthFormats =
	{
		vk::Format::eD16Unorm,
		vk::Format::eD16UnormS8Uint,
		vk::Format::eD24UnormS8Uint,
		vk::Format::eD32Sfloat,
		vk::Format::eD32SfloatS8Uint,
	};

	vk::Format depthFormat;
	v3d::vulkan::PhysicalDevice* pd = v3d::vulkan::PhysicalDevice::get();

	bool found = false;
	for (auto format : depthFormats)
	{
		if (pd->isFormatSupported( format, vk::ImageTiling::eOptimal, vk::FormatFeatureFlagBits::eDepthStencilAttachment ))
		{
			depthFormat = format;
			found = true;
			break;
		}
	}

	if (!found) return false;

	if (!depthImage->init( extent, depthFormat )) return false;

	return true;
}

vk::SurfaceFormatKHR v3d::vulkan::Swapchain::selectSurfaceFormat( const std::vector<vk::SurfaceFormatKHR>& surfaceFormats ) const
{
	if (surfaceFormats.size() == 1 && surfaceFormats.front().format == vk::Format::eUndefined) return { vk::Format::eB8G8R8A8Unorm , vk::ColorSpaceKHR::eSrgbNonlinear };

	for (const auto& surfaceFormat : surfaceFormats)
	{
		if (surfaceFormat.format == vk::Format::eB8G8R8A8Unorm && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) return surfaceFormat;
	}

	return surfaceFormats.front();
}

vk::Extent2D v3d::vulkan::Swapchain::selectExtent( const vk::SurfaceCapabilitiesKHR& surfaceCapabilities, const glm::ivec2& frameBufferSize ) const
{
	if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max() || surfaceCapabilities.currentExtent.height == std::numeric_limits<uint32_t>::max())
	{
		return { static_cast<uint32_t>(frameBufferSize.x) , static_cast<uint32_t>(frameBufferSize.y) };
	}
	else
	{
		return surfaceCapabilities.currentExtent;
	}
}

vk::PresentModeKHR v3d::vulkan::Swapchain::selectPresentMode( const std::vector<vk::PresentModeKHR>& presentModes ) const
{
	// No v-sync
	//return vk::PresentModeKHR::eImmediate;

	// V-sync (Always supported)
	return vk::PresentModeKHR::eFifo;

	// V-sync when faster than fps, immediate when slower than fps
	//return vk::PresentModeKHR::eFifoRelaxed;

	// Triple buffering
	//return vk::PresentModeKHR::eMailbox;
}

const vk::SwapchainKHR& Swapchain::getSwapchainKHR() const 
{ 
	return swapchain; 
}

const vk::Format& v3d::vulkan::Swapchain::getFormat() const 
{ 
	return surfaceFormat.format; 
}

const vk::Extent2D& v3d::vulkan::Swapchain::getExtent() const 
{ 
	return extent; 
}

const std::vector<vk::Image>& Swapchain::getImages() const 
{ 
	return images; 
}

const std::vector<vk::ImageView>& Swapchain::getImageViews() const 
{ 
	return imageViews; 
}

const v3d::vulkan::DepthImage* Swapchain::getDepthImage() const
{
	return depthImage;
}

VK_NS_END
V3D_NS_END