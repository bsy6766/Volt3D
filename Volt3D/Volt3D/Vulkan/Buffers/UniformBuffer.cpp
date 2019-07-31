/**
*	@file Buffer.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "UniformBuffer.h"

#include "Buffer.h"

V3D_NS_BEGIN
VK_NS_BEGIN

UniformBuffer::UniformBuffer( const uint32_t size, const vk::BufferUsageFlags usageFlags, const vk::MemoryPropertyFlags memoryPropertiyFlags, const void* data )
	: v3d::vulkan::Buffer( size, usageFlags, memoryPropertiyFlags, data )
{}

UniformBuffer::~UniformBuffer() {}

void UniformBuffer::update( const void* data )
{
	if (size == 0) return;
	void* mapPtr = mapMemory();
	if (mapPtr) std::memcpy( mapPtr, data, static_cast<std::size_t>(size) );
	unmapMemory();
}

V3D_NS_END
VK_NS_END