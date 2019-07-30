///**
//*	@file Texture.cpp
//*
//*	@author Seung Youp Baek
//*	@copyright Copyright (c) 2019 Seung Youp Baek
//*/
//
#include <PreCompiled.h>
//
//#include "Texture.h"
//
//#include "Devices.h"
//#include "Spritesheet/Image.h"
//#include "Texture.h"
//
//std::shared_ptr<v3d::vulkan::Devices> v3d::vulkan::Texture::devices = nullptr;
//
//v3d::vulkan::Texture::Texture()
//	: image()
//	, imageView()
//	, imageLayout()
//	, deviceMemory()
//	, sampler()
//{}
//
//v3d::vulkan::Texture::~Texture()
//{
//	release();
//}
//
//bool v3d::vulkan::Texture::init( const uint32_t width, const uint32_t height, const void* data, const uint64_t dataSize )
//{
//	if (v3d::vulkan::Texture::devices == nullptr) return false;
//	if (width == 0) return false;
//	if (height == 0) return false;
//	if (data == nullptr) return false;
//
//	// Create staging buffers
//	vk::Buffer stagingBuffer = devices->createBuffer( dataSize, vk::BufferUsageFlagBits::eTransferSrc );
//	vk::DeviceMemory stagingBufferMemory = devices->createDeviceMemory( stagingBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
//
//	vk::Device& ld = devices->logicalDevice;
//
//	// copy data
//	void* dataPtr = ld.mapMemory( stagingBufferMemory, 0, dataSize );
//	memcpy( dataPtr, data, dataSize );
//	ld.unmapMemory( stagingBufferMemory );
//
//	const vk::ImageViewType imageViewType = vk::ImageViewType::e2D;
//
//	createImage( width, height, vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled );
//	allocateAndBindMemoryToImage( vk::MemoryPropertyFlagBits::eDeviceLocal );
//
//	// Transition image layout
//	transitionImageLayout( texture.image, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );
//	copyBufferToImage( stagingBuffer, texture.image, uint32_t( texture.imageSource->getWidth() ), uint32_t( texture.imageSource->getHeight() ) );
//	transitionImageLayout( texture.image, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal );
//	
//	createImageView( vk::ImageViewType::e2D, vk::Format::eR8G8B8A8Unorm );
//
//	// Create sampler
//	vk::SamplerCreateInfo createInfo
//	(
//		vk::SamplerCreateFlags(),
//		vk::Filter::eLinear,
//		vk::Filter::eLinear,
//		vk::SamplerMipmapMode::eLinear,
//		vk::SamplerAddressMode::eMirroredRepeat,
//		vk::SamplerAddressMode::eMirroredRepeat,
//		vk::SamplerAddressMode::eMirroredRepeat,
//		0.0f,
//		false,
//		1,
//		false,
//		vk::CompareOp::eAlways,
//		0.0f,
//		0.0f,
//		vk::BorderColor::eIntOpaqueBlack,
//		false
//	);
//
//	sampler = ld.createSampler( createInfo, nullptr );
//
//	// Clean up
//	ld.destroyBuffer( stagingBuffer );
//	ld.freeMemory( stagingBufferMemory );
//}
//
//void v3d::vulkan::Texture::createImage( const uint32_t width, const uint32_t height, const vk::ImageType imageType, const vk::Format format, const vk::ImageTiling tilling, const vk::ImageUsageFlags usageFlags )
//{
//	vk::ImageCreateInfo createInfo
//	(
//		vk::ImageCreateFlags(),
//		imageType,
//		format,
//		vk::Extent3D( width, height, 1 ),
//		1u,
//		1u,
//		vk::SampleCountFlagBits::e1,
//		tilling,
//		usageFlags
//	);
//
//	image = devices->logicalDevice.createImage( createInfo );
//}
//
//void v3d::vulkan::Texture::allocateAndBindMemoryToImage( const vk::MemoryPropertyFlags memoryPropertyFlags )
//{
//	vk::MemoryRequirements memRequirements = devices->logicalDevice.getImageMemoryRequirements( image );
//	vk::MemoryAllocateInfo allocInfo
//	(
//		memRequirements.size,
//		devices->getMemoryTypeIndex( memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal )
//	);
//
//	deviceMemory = devices->logicalDevice.allocateMemory( allocInfo );
//	devices->logicalDevice.bindImageMemory( image, deviceMemory, 0 );
//}
//
//void v3d::vulkan::Texture::createImageView( const vk::ImageViewType imageViewType, const vk::Format format )
//{
//	// Create image view
//	vk::ImageViewCreateInfo createInfo
//	(
//		vk::ImageViewCreateFlags(),
//		image,
//		imageViewType,
//		format,
//		vk::ComponentMapping( vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA ),
//		vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 )
//	);
//
//	imageView = devices->logicalDevice.createImageView( createInfo, nullptr );
//}
//
//void v3d::vulkan::Texture::transitionImageLayout( const vk::Format& format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout )
//{
//}
//
//void v3d::vulkan::Texture::release()
//{
//	vk::Device& ld = devices->logicalDevice;
//	ld.destroySampler( sampler );
//	ld.freeMemory( deviceMemory );
//	ld.destroyImageView( imageView );
//	ld.destroyImage( image );
//}