/**
*	@file Instance.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Instance.h"

#include "Engine/Window.h"
#include "Debugs/DebugReportCallback.h"
#include "Debugs/DebugUtilsMessenger.h"
#include "Utils.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Instance::Instance()
	: instance( nullptr )
	, debugReportCallback( nullptr )
	, debugUtilsMessenger( nullptr )
{}

Instance::~Instance()
{
	SAFE_DELETE( debugUtilsMessenger );
	SAFE_DELETE( debugReportCallback );

	instance.destroy();
}

bool Instance::init( std::vector<const char*>& requiredExtensions, const bool validationLayerEnabled )
{
	vk::ApplicationInfo appInfo( "Learn Context", VK_MAKE_VERSION( 1, 0, 0 ), "Context", VK_MAKE_VERSION( 1, 0, 0 ), VK_API_VERSION_1_1 );

	std::vector<vk::ExtensionProperties> extensions = v3d::vulkan::utils::getExtensions();
	std::vector<vk::LayerProperties> layers = v3d::vulkan::utils::getLayers();

#ifdef BUILD_DEBUG
	auto& logger = v3d::Logger::getInstance();
	logger.logExtensions( extensions );
	logger.logLayers( layers );
#endif

	if (validationLayerEnabled)
	{
		requiredExtensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
		requiredExtensions.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
	}

	if (!vulkan::utils::checkExtensionProperties( extensions, requiredExtensions )) return false;

	std::vector<const char*> requiredLayers;
	if (validationLayerEnabled) requiredLayers.push_back( "VK_LAYER_KHRONOS_validation" );

	if (!vulkan::utils::checkLayerProperties( layers, requiredLayers )) return false;

	const vk::InstanceCreateInfo createInfo
	(
		{},
		&appInfo,
		uint32_t( requiredLayers.size() ),
		requiredLayers.data(),
		uint32_t( requiredExtensions.size() ),
		requiredExtensions.data()
	);

	instance = vk::createInstance( createInfo );

	if (validationLayerEnabled) if (!initDebugReport() || !initDebugUtilsMessenger()) return false;

	return true;
}

bool Instance::initDebugReport()
{
	debugReportCallback = new v3d::vulkan::DebugReportCallback();
	if (!debugReportCallback->init( instance )) return false;
	return true;
}

bool Instance::initDebugUtilsMessenger()
{
	debugUtilsMessenger = new v3d::vulkan::DebugUtilsMessenger();
	if (!debugUtilsMessenger->init( instance )) return false;
	return true;
}

const vk::Instance& Instance::getVKInstance() const { return instance; }

PFN_vkVoidFunction Instance::getProcAddr( const char* pName ) const { return instance.getProcAddr( pName ); }

inline vk::UniqueDebugReportCallbackEXT Instance::createDebugReportCallbackEXTUnique( const vk::DebugReportCallbackCreateInfoEXT& createInfo ) const
{
	return instance.createDebugReportCallbackEXTUnique( createInfo );
}

inline vk::UniqueDebugUtilsMessengerEXT Instance::createDebugUtilsMessengerEXTUnique( const vk::DebugUtilsMessengerCreateInfoEXT& createInfo ) const
{
	return instance.createDebugUtilsMessengerEXTUnique( createInfo );
}

inline std::vector<vk::PhysicalDevice> Instance::enumeratePhysicalDevices() const
{
	return instance.enumeratePhysicalDevices();
}

V3D_NS_END
VK_NS_END