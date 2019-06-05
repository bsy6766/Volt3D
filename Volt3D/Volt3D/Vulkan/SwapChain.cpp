/**
*	@file SwapChain.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "SwapChain.h"

#include "PhysicalDevice.h"
#include "Device.h"
#include "Surface.h"
#include "Core/Window.h"

v3d::vulkan::SwapChain::SwapChain()
	: swapChain()
	, surfaceFormat()
	, extent()
{}

bool v3d::vulkan::SwapChain::init(const v3d::vulkan::PhysicalDevice& physicalDevice, const v3d::vulkan::Device& device, const v3d::vulkan::Surface& surface, const v3d::glfw::Window& window)
{
	// surface format
	std::vector<vk::SurfaceFormatKHR> surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface);
	if (surfaceFormats.empty()) return false;
	surfaceFormat = selectSurfaceFormat(surfaceFormats);

	// extent2D
	const vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
	extent = selectExtent(surfaceCapabilities, window);

	std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
	if (presentModes.empty()) return false;
	const vk::PresentModeKHR swapchainPresentMode = selectPresentMode(presentModes);
	
	vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform;

	vk::CompositeAlphaFlagBitsKHR compositeAlpha =
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit : vk::CompositeAlphaFlagBitsKHR::eOpaque;

	vk::SwapchainCreateInfoKHR createInfo
	(
		vk::SwapchainCreateFlagsKHR(),
		reinterpret_cast<const vk::UniqueSurfaceKHR&>(surface).get(),
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

	swapChain = std::move(device.createSwapchainKHRUnique(createInfo));

	swapChainImages = device.getSwapchainImagesKHR(*this);

	imageViews.reserve(swapChainImages.size());
	vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
	vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
	for (auto image : swapChainImages)
	{
		vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, surfaceFormat.format, componentMapping, subResourceRange);
		imageViews.push_back(std::move(device.createImageViewUnique(imageViewCreateInfo)));
	}

	return true;
}

vk::SurfaceFormatKHR v3d::vulkan::SwapChain::selectSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& surfaceFormats) const
{
	if (surfaceFormats.size() == 1 && surfaceFormats.front().format == vk::Format::eUndefined) return { vk::Format::eB8G8R8A8Unorm , vk::ColorSpaceKHR::eSrgbNonlinear };
	
	for(const auto& surfaceFormat : surfaceFormats)
	{
		if (surfaceFormat.format == vk::Format::eB8G8R8A8Unorm && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) return surfaceFormat;
	}

	return surfaceFormats.front();
}

vk::Extent2D v3d::vulkan::SwapChain::selectExtent(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities, const v3d::glfw::Window& window) const
{
	if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max() || surfaceCapabilities.currentExtent.height == std::numeric_limits<uint32_t>::max())
	{
		const glm::uvec2 frameBufferSize = window.getFrameBufferSize();
		return { static_cast<uint32_t>(frameBufferSize.x) , static_cast<uint32_t>(frameBufferSize.y) };
	}
	else
	{
		return surfaceCapabilities.currentExtent;
	}
}

vk::PresentModeKHR v3d::vulkan::SwapChain::selectPresentMode(const std::vector<vk::PresentModeKHR>& presentModes) const
{
	return vk::PresentModeKHR::eFifo;
}

const vk::Format& v3d::vulkan::SwapChain::getFormat() const
{
	return surfaceFormat.format;
}

const vk::Extent2D& v3d::vulkan::SwapChain::getExtent2D() const
{
	return extent;
}

const std::size_t v3d::vulkan::SwapChain::getImageViewsSize() const
{
	return imageViews.size();
}

const std::vector<vk::UniqueImageView>& v3d::vulkan::SwapChain::getImageViews() const
{
	return imageViews;
}
