/**
*	@file Image.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Image.h"

#include "Vulkan/Context.h"
#include "Vulkan/Devices/LogicalDevice.h"
#include "Vulkan/Devices/PhysicalDevice.h"
#include "Vulkan/Commands/CommandBuffer.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Image::Image()
	: logicalDevice( v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice() )
	, image( nullptr )
	, deviceMemory( nullptr )
	, sampler( nullptr )
	, imageLayout()
	, imageView( nullptr )
	, extent()
	, format()
	//, filter()
	//, usageFlagBits()
	, mip_levels( 0 )
	, layers( 0 )
	//, sampleCount( vk::SampleCountFlagBits::e1 )
	//, samplerAddressMode()
	//, anisotropic( false )
{}

Image::~Image()
{
	logicalDevice.destroySampler( sampler );
	logicalDevice.destroyImageView( imageView );
	logicalDevice.destroyImage( image );
	logicalDevice.freeMemory( deviceMemory );
}

void Image::initImage( const uint32_t width, const uint32_t height, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags )
{
	vk::ImageCreateInfo imageCreateInfo
	(
		vk::ImageCreateFlags(),
		vk::ImageType::e2D,
		format,
		vk::Extent3D( width, height, 1 ),
		1u,
		1u,
		vk::SampleCountFlagBits::e1,
		tilling,
		usageFlags
	);

	image = logicalDevice.createImage( imageCreateInfo );

	vk::ImageViewCreateInfo imageViewCreateInfo
	(
		vk::ImageViewCreateFlags(),
		image,
		vk::ImageViewType::e2D,
		format,
		vk::ComponentMapping( vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA ),
		vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 )
	);

	imageView = logicalDevice.createImageView( imageViewCreateInfo, nullptr );
}

void Image::initImageDeviceMemory( const vk::MemoryPropertyFlags memoryPropertyFlags )
{
	vk::MemoryRequirements memRequirements = logicalDevice.getImageMemoryRequirements( image );

	vk::MemoryAllocateInfo allocInfo
	(
		memRequirements.size,
		v3d::vulkan::Context::get()->getPhysicalDevice()->getMemoryTypeIndex( memRequirements.memoryTypeBits, memoryPropertyFlags )
	);

	deviceMemory = logicalDevice.allocateMemory( allocInfo );
	logicalDevice.bindImageMemory( image, deviceMemory, 0 );
}

void Image::initSampler()
{
	vk::SamplerCreateInfo createInfo
	(
		vk::SamplerCreateFlags(),
		vk::Filter::eLinear,
		vk::Filter::eLinear,
		vk::SamplerMipmapMode::eLinear,
		vk::SamplerAddressMode::eMirroredRepeat,
		vk::SamplerAddressMode::eMirroredRepeat,
		vk::SamplerAddressMode::eMirroredRepeat,
		0.0f,
		false,
		1,
		false,
		vk::CompareOp::eAlways,
		0.0f,
		0.0f,
		vk::BorderColor::eIntOpaqueBlack,
		false
	);

	sampler = logicalDevice.createSampler( createInfo, nullptr );
}

void Image::transitionLayout( const vk::ImageLayout oldLayout, const vk::ImageLayout newLayout )
{
	auto cb = v3d::vulkan::CommandBuffer( vk::CommandBufferLevel::ePrimary );

	vk::ImageSubresourceRange subresourceRange
	(
		vk::ImageAspectFlagBits::eColor,
		0, 1,	// mipmap
		0, 1	// layer
	);

	vk::ImageMemoryBarrier barrier
	(
		vk::AccessFlags(),
		vk::AccessFlags(),
		oldLayout,
		newLayout,
		VK_QUEUE_FAMILY_IGNORED,
		VK_QUEUE_FAMILY_IGNORED,
		image,
		subresourceRange
	);
}

uint32_t Image::get_mip_levels( const vk::Extent2D& extent )
{
	return static_cast<uint32_t>(std::floor( std::log2( std::max( extent.width, extent.height ) ) ) + 1);
}

uint32_t Image::get_mip_levels( const vk::Extent3D& extent )
{
	return static_cast<uint32_t>(std::floor( std::log2( std::max( extent.width, std::max( extent.height, extent.depth ) ) ) ) + 1);
}

VK_NS_END
V3D_NS_END