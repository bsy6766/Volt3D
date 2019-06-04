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
	: imageAvailableSemaphore()
	, renderFinishedSemaphore()
{}

const vk::UniqueSemaphore& v3d::vulkan::Semaphore::getImageAvailableSemaphore() const
{
	return imageAvailableSemaphore;
}

const vk::UniqueSemaphore& v3d::vulkan::Semaphore::getRenderFinishedSemaphore() const
{
	return renderFinishedSemaphore;
}

bool v3d::vulkan::Semaphore::init(const v3d::vulkan::Device& device)
{
	imageAvailableSemaphore = device.createSemaphore(vk::SemaphoreCreateInfo(vk::SemaphoreCreateFlags()));
	renderFinishedSemaphore = device.createSemaphore(vk::SemaphoreCreateInfo(vk::SemaphoreCreateFlags()));

	return true;
}
