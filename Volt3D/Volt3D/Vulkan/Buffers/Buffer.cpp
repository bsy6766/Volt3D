/**
*	@file Buffer.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Buffer.h"

#include "Vulkan/Context.h"
#include "Vulkan/PhysicalDevice.h"
#include "Vulkan/LogicalDevice.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Buffer::Buffer( const vk::DeviceSize size, const vk::BufferUsageFlags usageFlags, const vk::MemoryPropertyFlags memoryPropertiyFlags, const void* data )
	: buffer()
	, deviceMemory()
	, size( size )
	, logicalDevice( v3d::vulkan::Context::get()->getLogicalDevice()->get() )
{
	initBuffer( usageFlags );
	initDeviceMemory( memoryPropertiyFlags );
	if (data) copyData( data );
}

Buffer::~Buffer()
{
	logicalDevice.destroyBuffer( buffer );
	logicalDevice.freeMemory( deviceMemory );
}

void * Buffer::mapMemory() const
{
	return logicalDevice.mapMemory( deviceMemory, 0, size );
}

void Buffer::unmapMemory() const
{
	logicalDevice.unmapMemory( deviceMemory );
}

void Buffer::initBuffer( const vk::BufferUsageFlags usageFlags )
{
	vk::BufferCreateInfo createInfo( vk::BufferCreateFlags(), size, usageFlags );
	buffer = logicalDevice.createBuffer( createInfo );
}

void Buffer::initDeviceMemory( const vk::MemoryPropertyFlags memoryPropertyFlags )
{
	const vk::Device& ld = logicalDevice;

	vk::MemoryRequirements memRequirements = ld.getBufferMemoryRequirements( buffer );
	vk::MemoryAllocateInfo allocInfo
	(
		memRequirements.size,
		v3d::vulkan::Context::get()->getPhysicalDevice()->getMemoryTypeIndex( memRequirements.memoryTypeBits, memoryPropertyFlags )
	);

	deviceMemory = ld.allocateMemory( allocInfo );
	ld.bindBufferMemory( buffer, deviceMemory, vk::DeviceSize( 0 ) );
}

void Buffer::copyData( const void* data )
{
	// @todo implement this
	//v3d::vulkan::Buffer stagingBuffer = v3d::vulkan::Buffer( size, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );
	//
	//const vk::Device& ld = logicalDevice;
	//void* mapPtr = ld.mapMemory( stagingBuffer.deviceMemory, 0, size );
	//memcpy( mapPtr, data, size );
	//ld.unmapMemory( stagingBuffer.deviceMemory );

	//auto cb = createCommandBuffer();
	//cb.begin( vk::CommandBufferUsageFlagBits::eOneTimeSubmit );
	//cb.copyBuffer( src, dst, size );
	//cb.end();
	//oneTimeSubmit( cb );

	//ld.freeCommandBuffers( commandPool, cb.getHandle() );
}

const vk::Buffer& Buffer::getBuffer() const
{
	return buffer;
}

const vk::DeviceMemory& Buffer::getDeviceMemory() const
{
	return deviceMemory;
}

const vk::DeviceSize& Buffer::getSize() const
{
	return size;
}

VK_NS_END
V3D_NS_END

