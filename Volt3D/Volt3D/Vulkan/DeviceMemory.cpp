/**
*	@file DeviceMemory.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "DeviceMemory.h"

#include "Device.h"
#include "PhysicalDevice.h"
#include "Buffer.h"

v3d::vulkan::DeviceMemory::DeviceMemory()
	: deviceMemory()
{}

bool v3d::vulkan::DeviceMemory::init(const v3d::vulkan::Device& device, const v3d::vulkan::PhysicalDevice& physicalDevice, const v3d::vulkan::Buffer& buffer)
{
	auto memRequirment = device.getMemoryRequirement(buffer);
	const vk::MemoryAllocateInfo allocInfo
	(
		memRequirment.size,
		physicalDevice.getMemoryTypeIndex(memRequirment.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
	);

	deviceMemory = device.allocateBuffer(allocInfo);

	return true;
}
