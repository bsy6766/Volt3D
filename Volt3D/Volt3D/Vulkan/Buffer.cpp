/**
*	@file Buffer.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Buffer.h"

#include "Device.h"
#include "Renderer/VertexData.h"

v3d::vulkan::Buffer::Buffer()
	: buffer(nullptr)
{}

bool v3d::vulkan::Buffer::init(const v3d::vulkan::Device& device, const v3d::VertexData& vertexData)
{
	vk::BufferCreateInfo createInfo
	(
		vk::BufferCreateFlags(),
		vertexData.getDataSize(),
		vk::BufferUsageFlagBits::eVertexBuffer
	);

	buffer = device.createBuffer(createInfo);

	return true;
}