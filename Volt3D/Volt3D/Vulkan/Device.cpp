#include <PreCompiled.h>

#include "Device.h"

#include <optional>

#include "Surface.h"
#include "PhysicalDevice.h"
#include "Utils.h"

Device::Device()
	: device()
	, graphicsQueueFamilyIndex(0)
	, presentQueueFamilyIndex(0)
{}

bool Device::init( PhysicalDevice & physicalDevice, Surface& surface )
{
	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

	auto& logger = Logger::getInstance();
	logger.info( "Total " + std::to_string( queueFamilyProperties.size() ) + " queue family properties found" );

	uint32_t i = 0;

	std::optional<uint32_t> gIndex = 0;
	std::optional<uint32_t> pIndex = 0;
	
	for( auto& queueFamilyProperty : queueFamilyProperties )
	{
		logger.trace( "Queue familty property: " + std::to_string( queueFamilyProperty.queueCount ) + " queues available" );
		logger.trace( "Queues: " + vk::to_string( queueFamilyProperty.queueFlags ) );

		if( queueFamilyProperty.queueCount > 0 && (queueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics) )
		{
			gIndex.emplace( i );
			logger.trace( "Found queue family property with GRAPHICS flag" );
		}

		vk::Bool32 supportSurface = physicalDevice.getSurfaceSupportKHR( i, surface );
		if( queueFamilyProperty.queueCount > 0 && supportSurface )
		{
			pIndex.emplace( i );
			logger.trace( "Found queue family property that supports surface KHR" );
		}

		if( gIndex.has_value() && pIndex.has_value() ) break;
		else
		{
			gIndex.reset();
			pIndex.reset();
		}

		i++;
	}

	if( !gIndex.has_value() || !pIndex.has_value() )
	{
		logger.critical( "Failed to find queue family property with GRAPHICS flag or that supports surface KHR" );
		return false;
	}

	graphicsQueueFamilyIndex = gIndex.value();
	presentQueueFamilyIndex = pIndex.value();

	const float queuePriority = 1.0f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo = vk::DeviceQueueCreateInfo
	(
		{},
		graphicsQueueFamilyIndex,
		1,
		&queuePriority
	);

	std::vector<vk::ExtensionProperties> extensions = physicalDevice.enumerateDeviceExtensionProperties();
#if _DEBUG
	logger.logExtensions( extensions );
#endif
	std::vector<const char*> requiredExtension = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	if( !vulkan::utils::checkExtensionProperties( extensions, requiredExtension ) ) return false;

	std::vector<vk::LayerProperties> layers = physicalDevice.enumerateDeviceLayerProperties();
#if _DEBUG
	logger.logLayers( layers );
#endif

	vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo( {}, 1, &deviceQueueCreateInfo, 0, nullptr, uint32_t( requiredExtension.size() ), requiredExtension.data() );
	device = std::move( physicalDevice.createLogicalDevice( createInfo ) );

	return true;
}
