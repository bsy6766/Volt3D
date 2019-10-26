/**
*	@file Texture.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Texture.h"

#include "TextureCache.h"
#include "Spritesheet/Image.h"
#include "Vulkan/Images/Image.h"
#include "vulkan/Buffers/Buffer.h"
#include "Vulkan/Devices/LogicalDevice.h"

V3D_NS_BEGIN

std::size_t Texture::idCounter = 1;

Texture::Texture()
	: id( 0 )
	, name()
	, image( nullptr )
{}

Texture::Texture( const std::string& name )
	: id( 0 )
	, name( name )
	, image( nullptr )
{}

Texture::~Texture() 
{
	SAFE_DELETE( image );
}

Texture* Texture::create( const std::string& name, const std::filesystem::path& textureFilePath, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags memoryProperty )
{
	v3d::Texture* newTexture = new (std::nothrow) v3d::Texture( name );
	if (newTexture)
	{
		if (newTexture->init( textureFilePath, tilling, usage, memoryProperty)) 
			return newTexture;

		SAFE_DELETE( newTexture );
	}

	return nullptr;
}

bool Texture::initImage( const vk::Extent3D& extent, const vk::Format& format )
{
	image = new (std::nothrow) v3d::vulkan::Image( extent, format );
	return image != nullptr;
}

bool Texture::init( const std::filesystem::path& textureFilePath, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags memoryProperty )
{
	// 1. Load image from texture file
	v3d::Image* imgSrc = v3d::Image::createPNG( textureFilePath );
	if (!imgSrc) return false;

	// 2. Create vulkan image
	if (!initImage( vk::Extent3D( imgSrc->getWidth(), imgSrc->getHeight(), 1 ), imgSrc->getVKFormat() ))
	{
		SAFE_DELETE( imgSrc );
		return false;
	}

	// 3. Initialize vulkan image
	image->initImage( tilling, usage );
	image->initImageDeviceMemory( memoryProperty );
	image->initImageView();
	image->initSampler();

	// 3. Create staging buffer
	const unsigned int imgDataSize = imgSrc->getDataSize();
	void* imgDataPtr = imgSrc->getData();

	const vk::Device& logicalDevice = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice();

	v3d::vulkan::Buffer stagingBuffer = v3d::vulkan::Buffer( imgDataSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

	void* bufferDataPtr = logicalDevice.mapMemory( stagingBuffer.getDeviceMemory(), 0, imgDataSize );
	memcpy( bufferDataPtr, imgDataPtr, imgDataSize );
	logicalDevice.unmapMemory( stagingBuffer.getDeviceMemory() );
	
	// 4. Buffer to image
	image->transitionLayout( vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer );
	image->copyBuffer( stagingBuffer.getBuffer() );
	image->transitionLayout( vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader );

	// 5. Assign ID
	id = v3d::Texture::idCounter++;

	// 6. Add to texture manager
	// @todo Maybe do something with return value?
	addToTextureManager();

	SAFE_DELETE( imgSrc );

	return true;
}

bool Texture::addToTextureManager()
{
	return v3d::TextureCache::get().addTexture( std::move(std::shared_ptr<v3d::Texture>(this)));
}

bool Texture::isValid() const
{
	return (image != nullptr) && id != 0;
}

void Texture::release()
{
	SAFE_DELETE( image );
}

std::size_t Texture::getID() const 
{
	return id; 
}

std::string Texture::getName() const 
{
	return name; 
}

uint32_t Texture::getWidth() const 
{
	return (image == nullptr) ? 0 : image->getWidth(); 
}

uint32_t Texture::getHeight() const 
{
	return (image == nullptr) ? 0 : image->getHeight(); 
}

uint32_t Texture::getDepth() const 
{
	return (image == nullptr) ? 0 : image->getDepth(); 
}

v3d::vulkan::Image* Texture::getImage() const 
{
	return image; 
}

void Texture::log() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.trace( "[Texture] info" );
	logger.trace( "ID: {}", id );
	logger.trace( "Name: {}", name );
	logger.trace( "Width: {}", image->extent.width );
	logger.trace( "Height: {}", image->extent.height );
	logger.trace( "Depth: {}", image->extent.depth );
}

V3D_NS_END;