/**
*	@file Fence.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Fence.h"

#include "Device.h"

v3d::vulkan::Fence::Fence()
	: fence()
{}

bool v3d::vulkan::Fence::init(const v3d::vulkan::Device& device)
{
	fence = device.createFence(vk::FenceCreateInfo(vk::FenceCreateFlags(vk::FenceCreateFlagBits::eSignaled)));
	return true;
}
