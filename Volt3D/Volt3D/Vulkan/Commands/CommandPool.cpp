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
	: logicalDevice( v3d::vulkan::Context::get()->getLogicalDevice()->get() )
	, commandPool( nullptr )
{
	vk::CommandPoolCreateInfo createInfo
	(
		vk::CommandPoolCreateFlags(),
		queueIndex
	);

	commandPool = logicalDevice.createCommandPool( createInfo );
}

CommandPool::~CommandPool()
{
	logicalDevice.destroyCommandPool( commandPool );
}

const vk::CommandPool& CommandPool::get() const
{
	return commandPool;
}

VK_NS_END
V3D_NS_END

