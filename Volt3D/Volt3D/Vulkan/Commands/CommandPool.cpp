/**
*	@file CommandPool.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "CommandPool.h"

#include "Vulkan/Context.h"
#include "Vulkan/Devices/LogicalDevice.h"

V3D_NS_BEGIN
VK_NS_BEGIN

CommandPool::CommandPool( const uint32_t queueIndex )
	: commandPool( nullptr )
{
	vk::CommandPoolCreateInfo createInfo
	(
		vk::CommandPoolCreateFlags(),
		queueIndex
	);

	commandPool = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice().createCommandPool( createInfo );
}

CommandPool::~CommandPool()
{
	v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice().destroyCommandPool( commandPool );
}

const vk::CommandPool& CommandPool::getVKCommandPool() const { return commandPool; }

VK_NS_END
V3D_NS_END

