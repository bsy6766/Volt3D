﻿/**
*	@file Context.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Context.h"

#include "Core/Window.h"
#include "vulkan/Instance.h"
#include "Vulkan/DebugReportCallback.h"
#include "Vulkan/DebugUtilsMessenger.h"
#include "Vulkan/Surface.h"
#include "Vulkan/PhysicalDevice.h"
#include "Vulkan/Device.h"
//#include "SwapChain.h"
//#include "Shader.h"
#include "Utils.h"
#include "Config/BuildConfig.h"

v3d::vulkan::Context::Context()
	: instance(nullptr)
	, validationLayerEnabled(false)
	, debugReportCallback(nullptr)
	, debugUtilsMessenger(nullptr)
	, surface(nullptr)
	, physicalDevice(nullptr)
	//, device(nullptr)
	//, swapChain(nullptr)
{}

v3d::vulkan::Context::~Context()
{
	release();
}

bool v3d::vulkan::Context::init(const v3d::glfw::Window& window, const bool enableValidationLayer)
{
	// Init logger
	auto& logger = v3d::Logger::getInstance();

	// Get version
	uint32_t major, minor, patch;
	if (!vulkan::utils::getVersion(major, minor, patch)) { logger.critical("Failed to get Context version."); return false; }
	logger.trace("Context version: " + std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch));

	validationLayerEnabled = enableValidationLayer;

	if (!initInstance(window)) return false;
	if (!initSurface(window)) return false;
	if( !initPhysicalDevice() ) return false;
	if( !initDevice() ) return false;
	//if( !initSwapChain() ) return false;

	return true;
}

bool v3d::vulkan::Context::initInstance(const v3d::glfw::Window& window)
{
	auto& logger = Logger::getInstance();

	vk::ApplicationInfo appInfo("Learn Context", VK_MAKE_VERSION(1, 0, 0), "Context", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_1);

	std::vector<vk::ExtensionProperties> extensions = getExtensions();
#ifdef BUILD_DEBUG
	logger.logExtensions(extensions);
#endif

	std::vector<vk::LayerProperties> layers = getLayers();
#ifdef BUILD_DEBUG
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

	instance = new (std::nothrow) v3d::vulkan::Instance(std::move(vk::createInstanceUnique(createInfo)));
	if (instance == nullptr) { v3d::Logger::getInstance().bad_alloc<Instance>(); return false; }

	logger.trace("Created Context instance");

	if (validationLayerEnabled) if (!initDebugReport() || !initDebugUtilsMessenger()) return false;

	return true;
}

bool v3d::vulkan::Context::initDebugReport()
{
	debugReportCallback = new (std::nothrow) v3d::vulkan::DebugReportCallback();
	if (debugReportCallback == nullptr) { v3d::Logger::getInstance().bad_alloc<DebugReportCallback>(); return false; }
	if (!debugReportCallback->init(*instance)) return false;
	return true;
}

bool v3d::vulkan::Context::initDebugUtilsMessenger()
{
	debugUtilsMessenger = new (std::nothrow) v3d::vulkan::DebugUtilsMessenger();
	if (debugUtilsMessenger == nullptr) { v3d::Logger::getInstance().bad_alloc<DebugUtilsMessenger>(); return false; }
	if (!debugUtilsMessenger->init(*instance)) return false;
	return true;
}

bool v3d::vulkan::Context::initSurface( const v3d::glfw::Window& window )
{
	VkSurfaceKHR cVkSurfaceKHR;
	if( !window.createWindowSurface( *instance, cVkSurfaceKHR ) ) return false;

	surface = new(std::nothrow) v3d::vulkan::Surface(std::move(vk::UniqueSurfaceKHR(cVkSurfaceKHR, instance->getHandle().get())));
	if (surface == nullptr) v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Surface>();
	return true;
}

bool v3d::vulkan::Context::initPhysicalDevice()
{
	std::vector<vk::PhysicalDevice> physicalDevices = std::move(instance->enumeratePhysicalDevices());

	for (vk::PhysicalDevice& curPhysicalDevice : physicalDevices)
	{
		if (v3d::vulkan::PhysicalDevice::isSuitable(curPhysicalDevice))
		{
			physicalDevice = new (std::nothrow) v3d::vulkan::PhysicalDevice(std::move(curPhysicalDevice));
			if (physicalDevice == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::PhysicalDevice>(); return false; }
			return true;
		}
	}

	v3d::Logger::getInstance().critical("Failed to find suitable GPU with GRAPHICS BIT");
	return false;
}

bool v3d::vulkan::Context::initDevice()
{
	auto graphicsQueueFamilyIndex = physicalDevice->getGraphicsQueueFamilyIndex();

	const float queuePriority = 1.0f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo = vk::DeviceQueueCreateInfo
	(
		{},
		graphicsQueueFamilyIndex.value(),
		1,
		&queuePriority
	);

	//std::vector<vk::ExtensionProperties> extensions = physicalDevice->getHandle().enumerateDeviceExtensionProperties();
	physicalDevice->enumerateDeviceExtensionProperties();
//#ifdef BUILD_DEBUG
//	v3d::Logger::getInstance().logExtensions(extensions);
//#endif
//	std::vector<const char*> requiredExtension = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
//	if (!vulkan::utils::checkExtensionProperties(extensions, requiredExtension)) return false;
//
//	vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo
//	(
//		{},
//		1,
//		&deviceQueueCreateInfo,
//		0,
//		nullptr,
//		uint32_t(requiredExtension.size()),
//		requiredExtension.data()
//	);
//
//	device = new (std::nothrow) v3d::vulkan::Device(std::move(physicalDevice->createDeviceUnique(createInfo)));
//	if (device == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Device>(); return false; }
	return true;
}

/*
bool v3d::vulkan::Context::initSwapChain()
{
	swapChain = new (std::nothrow) SwapChain();
	if( !swapChain ) return false;
	return swapChain->init( *physicalDevice, *device, *surface );
}

bool v3d::vulkan::Context::initGraphicsPipeline()
{
	//Shader* vertShader = Shader::create( "Shaders/vert.spv", logicalDevice );
	//if( !vertShader ) return false;
	//Shader* fragShader = Shader::create( "Shaders/frag.spv", logicalDevice );
	//if( !fragShader ) return false;


	//const auto v = { Shader() };

	//vk::PipelineShaderStageCreateInfo vertCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, &(Shader()), "main" );

	//delete vertShader;
	//delete fragShader;

	return true;
}

*/

void v3d::vulkan::Context::release()
{
	auto& logger = v3d::Logger::getInstance();
	logger.info("Releasing Context...");
	//if( enableValidationLayer && debugCallback ) 
	//{ 
	//	delete debugCallback; 
	//	debugCallback = nullptr;
	//}
	logger.info("Releasing Context finished");
}

std::vector<vk::ExtensionProperties> v3d::vulkan::Context::getExtensions() const
{
	return vk::enumerateInstanceExtensionProperties();
}

std::vector<vk::LayerProperties> v3d::vulkan::Context::getLayers() const
{
	return vk::enumerateInstanceLayerProperties();
}