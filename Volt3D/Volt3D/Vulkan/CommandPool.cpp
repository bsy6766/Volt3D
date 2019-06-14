/**
*	@file CommandPool.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "CommandPool.h"

#include "PhysicalDevice.h"
#include "Device.h"

v3d::vulkan::CommandPool::CommandPool()
	: commandPool()
{}

bool v3d::vulkan::CommandPool::init(const v3d::vulkan::PhysicalDevice& physicalDevice, const v3d::vulkan::Device& device)
{
	uint32_t graphicsFamilyIndex = physicalDevice.getGraphicsQueueFamilyIndex();

	vk::CommandPoolCreateInfo createInfo
	(
		vk::CommandPoolCreateFlags(),
		graphicsFamilyIndex
	);

	commandPool = device.createCommandPool(createInfo);

	return true;
}