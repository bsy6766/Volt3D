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

v3d::vulkan::Queue::Queue()
	: graphicsQueue()
	, presentQueue()
{}

bool v3d::vulkan::Queue::init(const v3d::vulkan::PhysicalDevice & physicalDevice, const v3d::vulkan::Device & device)
{
	graphicsQueue = device.getQueue(physicalDevice.getGraphicsQueueFamilyIndex(), 0);
	presentQueue = device.getQueue(physicalDevice.getPresentQueueFamilyIndex(), 0);
	return true;
}

const vk::Queue& v3d::vulkan::Queue::getGraphicsQueue() const
{
	return graphicsQueue;
}

const vk::Queue& v3d::vulkan::Queue::getPresentQueue() const
{
	return presentQueue;
}

void v3d::vulkan::Queue::submit(const vk::SubmitInfo& submitInfo) const
{
	graphicsQueue.submit(submitInfo, nullptr);
}

void v3d::vulkan::Queue::present(const vk::PresentInfoKHR& presentInfo) const
{
	presentQueue.presentKHR(presentInfo);
}
