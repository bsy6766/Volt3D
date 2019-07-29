/**
*	@file Texture.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Texture.h"

#include "Devices.h"
#include "Spritesheet/Image.h"

v3d::vulkan::Texture::Texture(vk::Device& logicalDevice )
	: logicalDevice( logicalDevice )
	, path(nullptr)
	, image()
	, imageView()
	, imageLayout()
	, deviceMemory()
	, sampler()
	, width(0)
	, height(0)
	, channels(0)
	, mipLevel(0)
	, layerCount(0)
{}

v3d::vulkan::Texture::~Texture()
{
	release();
}

void v3d::vulkan::Texture::release()
{
	logicalDevice.destroySampler( sampler );
	logicalDevice.freeMemory( deviceMemory );
	logicalDevice.destroyImageView( imageView );
	logicalDevice.destroyImage( image );
}






v3d::vulkan::Texture2D::Texture2D( vk::Device& device )
	: v3d::vulkan::Texture(device)
{}

bool v3d::vulkan::Texture2D::init( const char* filePath, v3d::vulkan::Devices* devices )
{
	// path is guarnateed not to be null
	v3d::Image* imageSource = v3d::Image::createPNG( filePath );
	if (imageSource == nullptr) return false;

	path = path;
	width = imageSource->getWidth();
	height = imageSource->getHeight();
	channels = imageSource->getHeight();
	mipLevel = 1;
	layerCount = 1;

	createImage( devices, vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal );


	return false;
}

void v3d::vulkan::Texture2D::createImage( v3d::vulkan::Devices* devices, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags, const vk::MemoryPropertyFlags memoryPropertyFlags )
{
	vk::ImageCreateInfo createInfo
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

	image = logicalDevice.createImage( createInfo );
	vk::MemoryRequirements memRequirements = logicalDevice.getImageMemoryRequirements( image );

	vk::MemoryAllocateInfo allocInfo
	(
		memRequirements.size,
		devices->getMemoryTypeIndex( memRequirements.memoryTypeBits, memoryPropertyFlags )
	);

	deviceMemory = logicalDevice.allocateMemory( allocInfo );
	logicalDevice.bindImageMemory( image, deviceMemory, 0 );
}

std::shared_ptr<v3d::vulkan::Texture2D> v3d::vulkan::Texture2D::create( const char* filePath, v3d::vulkan::Devices* devices )
{
	if (filePath == nullptr) return nullptr;
	if (devices == nullptr) return nullptr;

	v3d::vulkan::Texture2D* newTexture2D = new (std::nothrow) v3d::vulkan::Texture2D( devices->logicalDevice );
	if (newTexture2D)
	{
		if (newTexture2D->init( filePath, devices )) return std::shared_ptr<v3d::vulkan::Texture2D>( newTexture2D );
		else delete newTexture2D;
	}

	return nullptr;
}
