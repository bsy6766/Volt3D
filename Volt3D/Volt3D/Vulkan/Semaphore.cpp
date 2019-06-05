/**
*	@file Semaphore.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Semaphore.h"

#include "Device.h"

v3d::vulkan::Semaphore::Semaphore()
	: semaphore()
{}

bool v3d::vulkan::Semaphore::init(const v3d::vulkan::Device& device)
{
	semaphore = device.createSemaphore(vk::SemaphoreCreateInfo(vk::SemaphoreCreateFlags()));
	return true;
}
