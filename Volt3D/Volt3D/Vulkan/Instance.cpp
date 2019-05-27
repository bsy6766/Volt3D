/**
*	@file Instance.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Instance.h"

v3d::vulkan::Instance::Instance(vk::UniqueInstance&& instance)
	: instance(std::move(instance))
{}

PFN_vkVoidFunction v3d::vulkan::Instance::getProcAddr(const char* pName) const
{
	return instance.get().getProcAddr(pName);
}

inline vk::UniqueDebugReportCallbackEXT v3d::vulkan::Instance::createDebugReportCallbackEXTUnique(const vk::DebugReportCallbackCreateInfoEXT& createInfo) const
{
	return std::move(instance.get().createDebugReportCallbackEXTUnique(createInfo));
}

inline vk::UniqueDebugUtilsMessengerEXT v3d::vulkan::Instance::createDebugUtilsMessengerEXTUnique(const vk::DebugUtilsMessengerCreateInfoEXT& createInfo) const
{
	return std::move(instance.get().createDebugUtilsMessengerEXTUnique(createInfo));
}

inline std::vector<vk::PhysicalDevice> v3d::vulkan::Instance::enumeratePhysicalDevices() const
{
	return std::move(instance->enumeratePhysicalDevices());
}
