/**
*	@file Queue.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Queue.h"

#include "PhysicalDevice.h"

v3d::vulkan::Queue::Queue()
	: queue()
{}

bool v3d::vulkan::Queue::init(const vk::Device& device, const uint32_t familyIndex)
{
	queue = device.getQueue(familyIndex, 0);
	return true;
}

void v3d::vulkan::Queue::submit(const vk::SubmitInfo& submitInfo) const
{
	queue.submit(submitInfo, nullptr);
}

void v3d::vulkan::Queue::submit(const vk::SubmitInfo& submitInfo, const vk::Fence& fence) const
{
	queue.submit(submitInfo, fence);
}

vk::Result v3d::vulkan::Queue::present(const vk::PresentInfoKHR& presentInfo) const
{
	try
	{
		return queue.presentKHR(presentInfo);
	}
	catch (vk::OutOfDateKHRError&)
	{
		return vk::Result::eErrorOutOfDateKHR;
	}
}

void v3d::vulkan::Queue::waitIdle() const
{
	queue.waitIdle();
}
