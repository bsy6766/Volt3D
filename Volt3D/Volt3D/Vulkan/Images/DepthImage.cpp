/**
*	@file DepthImage.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "DepthImage.h"

#include "Vulkan/Devices/LogicalDevice.h"
#include "Vulkan/Devices/PhysicalDevice.h"

V3D_NS_BEGIN
VK_NS_BEGIN

DepthImage::DepthImage()
	: image()
	, imageView()
	, deviceMemory()
{}

DepthImage::~DepthImage()
{
	const vk::Device& logicalDevice = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice();
	logicalDevice.destroyImageView( imageView );
	logicalDevice.destroyImage( image );
	logicalDevice.freeMemory( deviceMemory );
}

bool DepthImage::init( const vk::Extent2D& extent, const vk::Format& format, const vk::ImageTiling tiling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags memProperties )
{
	if (extent.width == 0 || extent.height == 0) 
		return false;

	if (format != vk::Format::eD16Unorm &&
		format != vk::Format::eD16UnormS8Uint &&
		format != vk::Format::eD24UnormS8Uint &&
		format != vk::Format::eD32Sfloat &&
		format != vk::Format::eD32SfloatS8Uint)
		return false;

	// 0. Create Image
	const vk::ImageCreateInfo imageCI =
	{
		vk::ImageCreateFlags(),
		vk::ImageType::e2D,
		format,
		vk::Extent3D( extent, 1 ),
		1,
		1,
		vk::SampleCountFlagBits::e1,
		tiling,
		usage,
	};

	const vk::Device& logicalDevice = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice();
	image = logicalDevice.createImage( imageCI );

	// 1. Bind DeviceMemory
	vk::MemoryRequirements memRequirements = logicalDevice.getImageMemoryRequirements( image );

	vk::MemoryAllocateInfo memoryAI
	(
		memRequirements.size,
		v3d::vulkan::PhysicalDevice::get()->getMemoryTypeIndex( memRequirements.memoryTypeBits, memProperties )
	);

	deviceMemory = logicalDevice.allocateMemory( memoryAI );
	logicalDevice.bindImageMemory( image, deviceMemory, 0 );
	
	// 2. Create Image View

	vk::ImageViewCreateInfo imageViewCI
	(
		vk::ImageViewCreateFlags(),
		image,
		vk::ImageViewType::e2D,
		format,
		vk::ComponentMapping( vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA ),
		vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1 )
	);

	imageView = logicalDevice.createImageView( imageViewCI, nullptr );

	return true;
}

const vk::ImageView& DepthImage::getImageView() const
{
	return imageView;
}

VK_NS_END
V3D_NS_END