/**
*	@file DebugReportCallback.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "DebugReportCallback.h"

#include "Instance.h"

PFN_vkCreateDebugReportCallbackEXT  pfnVkCreateDebugReportCallbackEXT = VK_NULL_HANDLE;
PFN_vkDestroyDebugReportCallbackEXT pfnVkDestroyDebugReportCallbackEXT = VK_NULL_HANDLE;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
	return pfnVkCreateDebugReportCallbackEXT(instance, pCreateInfo, pAllocator, pCallback);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
	pfnVkDestroyDebugReportCallbackEXT(instance, callback, pAllocator);
}

v3d::vulkan::DebugReportCallback::DebugReportCallback()
	: debugReportCallback(nullptr)
{}

bool v3d::vulkan::DebugReportCallback::init(const v3d::vulkan::Instance& instance)
{
	auto& logger = v3d::Logger::getInstance();

	pfnVkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(instance.getProcAddr("vkCreateDebugReportCallbackEXT"));
	pfnVkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(instance.getProcAddr("vkDestroyDebugReportCallbackEXT"));

	if (!pfnVkCreateDebugReportCallbackEXT)
	{
		logger.error("vkCreateDebugReportCallbackEXT is not supported");
		return false;
	}
	if (!pfnVkDestroyDebugReportCallbackEXT)
	{
		logger.error("vkDestroyDebugReportCallbackEXT is not supported");
		return false;
	}

	const vk::DebugReportFlagsEXT flags
	(
		vk::DebugReportFlagBitsEXT::eWarning | 
		vk::DebugReportFlagBitsEXT::ePerformanceWarning | 
		vk::DebugReportFlagBitsEXT::eError |
		vk::DebugReportFlagBitsEXT::eDebug
	);

	debugReportCallback = std::move(instance.createDebugReportCallbackEXTUnique(vk::DebugReportCallbackCreateInfoEXT(flags, debugReportCallbackFunc, this)));

	return true;
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL v3d::vulkan::DebugReportCallback::debugReportCallbackFunc(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT object_type, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
	Logger::getInstance().debug(std::string("Debug report: ") + std::string(pMessage));
	return VK_FALSE;
}