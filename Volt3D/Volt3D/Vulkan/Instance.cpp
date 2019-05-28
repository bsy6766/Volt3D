/**
*	@file Instance.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Instance.h"

#include "Core/Window.h"
#include "Utils.h"

v3d::vulkan::Instance::Instance()
	: instance()
{}

bool v3d::vulkan::Instance::init(const v3d::glfw::Window& window, const bool validationLayerEnabled)
{
	vk::ApplicationInfo appInfo("Learn Context", VK_MAKE_VERSION(1, 0, 0), "Context", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_1);

	std::vector<vk::ExtensionProperties> extensions = v3d::vulkan::utils::getExtensions();
	std::vector<vk::LayerProperties> layers = v3d::vulkan::utils::getLayers();

#ifdef BUILD_DEBUG
	auto& logger = v3d::Logger::getInstance();
	logger.logExtensions(extensions);
	logger.logLayers(layers);
#endif

	std::vector<const char*> requiredExtensions;
	window.getGLFWVKExtensions(requiredExtensions);

	if (validationLayerEnabled)
	{
		requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		requiredExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	if (!vulkan::utils::checkExtensionProperties(extensions, requiredExtensions)) return false;

	std::vector<const char*> requiredLayers;
	if (validationLayerEnabled) requiredLayers.push_back("VK_LAYER_KHRONOS_validation");

	if (!vulkan::utils::checkLayerProperties(layers, requiredLayers)) return false;

	const vk::InstanceCreateInfo createInfo
	(
		{},
		&appInfo,
		uint32_t(requiredLayers.size()),
		requiredLayers.data(),
		uint32_t(requiredExtensions.size()),
		requiredExtensions.data()
	);

	instance = std::move(vk::createInstanceUnique(createInfo));

	return true;
}

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
