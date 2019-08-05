﻿/**
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

V3D_NS_BEGIN
VK_NS_BEGIN

Image::Image()
	: logicalDevice( v3d::vulkan::Context::get()->getLogicalDevice()->get() )
{}

void Image::initImage( const uint32_t w, const uint32_t h, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags )
{
	vk::ImageCreateInfo createInfo
	(
		vk::ImageCreateFlags(),
		vk::ImageType::e2D,
		format,
		vk::Extent3D( w, h, 1 ),
		1u,
		1u,
		vk::SampleCountFlagBits::e1,
		tilling,
		usageFlags
	);

	image = logicalDevice.createImage( createInfo );
}

void Image::initImageDeviceMemory( const vk::MemoryPropertyFlags memoryPropertyFlags )
{
	vk::MemoryRequirements memRequirements = logicalDevice.getImageMemoryRequirements( image );

	vk::MemoryAllocateInfo allocInfo
	(
		memRequirements.size,
		v3d::vulkan::Context::get()->getPhysicalDevice()->getMemoryTypeIndex( memRequirements.memoryTypeBits, memoryPropertyFlags )
	);

	imageDeviceMemory = logicalDevice.allocateMemory( allocInfo );
	logicalDevice.bindImageMemory( image, imageDeviceMemory, 0 );
}

Image::~Image()
{
	logicalDevice.destroySampler( sampler );
	logicalDevice.destroyImageView( imageView );
	logicalDevice.destroyImage( image );
	logicalDevice.freeMemory( imageDeviceMemory );
}

VK_NS_END
V3D_NS_END