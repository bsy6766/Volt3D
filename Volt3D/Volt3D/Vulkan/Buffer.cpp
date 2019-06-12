/**
*	@file Buffer.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Buffer.h"

#include "Device.h"

v3d::vulkan::Buffer::Buffer()
	: buffer(nullptr)
{}

bool v3d::vulkan::Buffer::init(const v3d::vulkan::Device& device, const uint64_t size, const vk::BufferUsageFlags usageFlag)
{
	vk::BufferCreateInfo createInfo
	(
		vk::BufferCreateFlags(),
		size,
		usageFlag
	);

	buffer = device.createBuffer(createInfo);

	return true;
}