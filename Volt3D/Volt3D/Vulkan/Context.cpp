/**
*	@file Context.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Context.h"

#include "Core/Window.h"
#include "Instance.h"
#include "DebugReportCallback.h"
#include "DebugUtilsMessenger.h"
#include "Surface.h"
#include "PhysicalDevice.h"
#include "Device.h"
#include "SwapChain.h"
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
	, device(nullptr)
	, swapChain(nullptr)
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
			physicalDevice = new (std::nothrow) v3d::vulkan::PhysicalDevice(std::move(curPhysicalDevice), *surface);
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
	if (!graphicsQueueFamilyIndex.has_value()) return false;

	const float queuePriority = 1.0f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo = vk::DeviceQueueCreateInfo
	(
		{},
		graphicsQueueFamilyIndex.value(),
		1,
		&queuePriority
	);

	std::vector<vk::ExtensionProperties> extensions = physicalDevice->EnumerateDeviceExtensionProperties();

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

	device = new (std::nothrow) v3d::vulkan::Device(std::move(physicalDevice->createDeviceUnique(createInfo)));
	if (device == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Device>(); return false; }

	return true;
}

bool v3d::vulkan::Context::initSwapChain()
{
	std::vector<vk::SurfaceFormatKHR> formats = physicalDevice->getSurfaceFormatsKHR(*surface);
	if (formats.empty()) return false;

	vk::Format format = (formats.front().format == vk::Format::eUndefined) ? vk::Format::eB8G8R8A8Unorm : formats.front().format;

	vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice->getSurfaceCapabilitiesKHR(*surface);
	vk::Extent2D swapChainExtent;
	if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max())
	{
		// If the surface size is undefined, the size is set to the size of the images requested.
		swapChainExtent.width = std::clamp(1280u, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);
		swapChainExtent.height = std::clamp(720u, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
	}
	else
	{
		// If the surface size is defined, the swap chain size must match
		swapChainExtent = surfaceCapabilities.currentExtent;
	}

	// The FIFO present mode is guaranteed by the spec to be supported
	vk::PresentModeKHR swapchainPresentMode = vk::PresentModeKHR::eFifo;
	// Visit again to learn more about present mode.

	vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform;

	vk::CompositeAlphaFlagBitsKHR compositeAlpha =
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit : vk::CompositeAlphaFlagBitsKHR::eOpaque;

	vk::SwapchainCreateInfoKHR createInfo
	(
		vk::SwapchainCreateFlagsKHR(),
		reinterpret_cast<const vk::UniqueSurfaceKHR&>(surface).get(),
		surfaceCapabilities.minImageCount,
		format,
		vk::ColorSpaceKHR::eSrgbNonlinear,
		swapChainExtent,
		1,
		vk::ImageUsageFlagBits::eColorAttachment,
		vk::SharingMode::eExclusive,
		0,
		nullptr,
		preTransform,
		compositeAlpha,
		swapchainPresentMode,
		true,
		nullptr
	);

	/*
	uint32_t queueFamilyIndices[2] = { physicalDevice.getGraphicsQueueFamilyIndex(), physicalDevice.getPresentQueueFamilyIndex() };
	if( queueFamilyIndices[0] != queueFamilyIndices[1] )
	{
		// If the graphics and present queues are from different queue families, we either have to explicitly transfer ownership of images between
		// the queues, or we have to create the swapchain with imageSharingMode as VK_SHARING_MODE_CONCURRENT
		createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	*/

	swapChain = new (std::nothrow) v3d::vulkan::SwapChain(std::move(device->createSwapchainKHRUnique(createInfo)));
	if (swapChain == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::SwapChain>(); return false; }

	return true;
}

/*
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