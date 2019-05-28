/**
*	@file Device.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Device.h"

v3d::vulkan::Device::Device(vk::UniqueDevice&& device)
	: device(std::move(device))
{}

vk::UniqueSwapchainKHR v3d::vulkan::Device::createSwapchainKHRUnique(vk::SwapchainCreateInfoKHR & createInfo) const
{
	return std::move(device->createSwapchainKHRUnique(createInfo));
}

vk::UniqueShaderModule v3d::vulkan::Device::createShaderModuleUnique(std::vector<char> buffer) const
{
	return std::move(device->createShaderModuleUnique(vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), buffer.size(), reinterpret_cast<const uint32_t*>(buffer.data()))));
}