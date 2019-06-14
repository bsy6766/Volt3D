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

vk::CommandPool::CommandPool()
	: commandPool()
{}

bool vk::CommandPool::init(const v3d::vulkan::PhysicalDevice& physicalDevice, const v3d::vulkan::Device& device)
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