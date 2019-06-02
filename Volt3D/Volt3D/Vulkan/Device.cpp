/**
*	@file Device.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Device.h"

#include "PhysicalDevice.h"
#include "SwapChain.h"
#include "Utils.h"

v3d::vulkan::Device::Device()
	: device()
{}

bool v3d::vulkan::Device::init(v3d::vulkan::PhysicalDevice& physicalDevice)
{
	auto graphicsQueueFamilyIndex = physicalDevice.getGraphicsQueueFamilyIndex();

	const float queuePriority = 1.0f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo = vk::DeviceQueueCreateInfo
	(
		{},
		graphicsQueueFamilyIndex,
		1,
		&queuePriority
	);

	std::vector<vk::ExtensionProperties> extensions = physicalDevice.EnumerateDeviceExtensionProperties();

#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().logExtensions(extensions);
#endif
	std::vector<const char*> requiredExtension = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	if (!vulkan::utils::checkExtensionProperties(extensions, requiredExtension)) return false;

	vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo
	(
		{},
		1,
		&deviceQueueCreateInfo,
		0,
		nullptr,
		uint32_t(requiredExtension.size()),
		requiredExtension.data()
	);

	device = physicalDevice.createDeviceUnique(createInfo);

	return true;
}

vk::UniqueSwapchainKHR v3d::vulkan::Device::createSwapchainKHRUnique(const vk::SwapchainCreateInfoKHR & createInfo) const
{
	return device->createSwapchainKHRUnique(createInfo);
}

vk::UniqueShaderModule v3d::vulkan::Device::createShaderModuleUnique(std::vector<char> buffer) const
{
	return device->createShaderModuleUnique(vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), buffer.size(), reinterpret_cast<const uint32_t*>(buffer.data())));
}

std::vector<vk::Image> v3d::vulkan::Device::getSwapchainImagesKHR(const v3d::vulkan::SwapChain& swapChain) const
{
	return device->getSwapchainImagesKHR(swapChain.get());
}

vk::UniqueImageView v3d::vulkan::Device::createImageViewUnique(const vk::ImageViewCreateInfo& createInfo) const
{
	return device->createImageViewUnique(createInfo);
}
