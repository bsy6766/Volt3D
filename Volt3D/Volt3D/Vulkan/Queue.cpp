/**
*	@file Queue.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Queue.h"

#include "PhysicalDevice.h"
#include "Device.h"
#include "Fence.h"

v3d::vulkan::Queue::Queue()
	: queue()
{}

bool v3d::vulkan::Queue::init(const v3d::vulkan::Device & device, const uint32_t familyIndex)
{
	queue = device.getQueue(familyIndex, 0);
	return true;
}

void v3d::vulkan::Queue::submit(const vk::SubmitInfo& submitInfo) const
{
	queue.submit(submitInfo, nullptr);
}

void v3d::vulkan::Queue::submit(const vk::SubmitInfo& submitInfo, const v3d::vulkan::Fence& fence) const
{
	queue.submit(submitInfo, fence.get());
}

void v3d::vulkan::Queue::present(const vk::PresentInfoKHR& presentInfo) const
{
	queue.presentKHR(presentInfo);
}

void v3d::vulkan::Queue::waitIdle() const
{
	queue.waitIdle();
}
