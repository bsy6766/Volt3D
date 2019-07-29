/**
*	@file Devices.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Devices.h"

#include "Utils.h"
#include "Utils/Logger.h"
#include "Config/BuildConfig.h"

v3d::vulkan::Devices::Devices()
	: physicalDevice()
	, logicalDevice()
	, properties()
	, features()
	, memoryProperties()
	, QFI( { 0,0,0,0 } )
{}

bool v3d::vulkan::Devices::isSuitablePhysicalDevice( const vk::PhysicalDevice& physicalDevice )
{
	auto& logger = v3d::Logger::getInstance();

	static auto bitToStr = []( uint64_t size ) -> std::string
	{
		const std::vector<std::string> SUFFIXES{ { "B", "KB", "MB", "GB", "TB", "PB" } };
		size_t suffixIndex = 0;
		while (suffixIndex < SUFFIXES.size() - 1 && size > 1024) {
			size >>= 10;
			++suffixIndex;
		}

		std::stringstream buffer;
		buffer << size << " " << SUFFIXES[suffixIndex];
		return buffer.str();
	};

	vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();

	if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
	{
		logger.info( "Found discrete GPU" );

		auto physicalDeviceMemoryProperties = physicalDevice.getMemoryProperties();

		logger.trace( "GPU information ---" );
		logger.trace( "Driver Version: " + std::to_string( properties.driverVersion ) );
		logger.trace( "Device Name: " + std::string( properties.deviceName ) );
		logger.trace( "Device Type: VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU" );
		const int major = (properties.apiVersion >> 22) & 0x3FF;
		const int minor = (properties.apiVersion >> 12) & 0x3FF;
		const int version = (properties.apiVersion & 0xFFF);
		logger.trace( "API Version: " + std::to_string( major ) + "." + std::to_string( minor ) + "." + std::to_string( version ) );
		logger.trace( "Memory heaps: " + std::to_string( physicalDeviceMemoryProperties.memoryHeapCount ) );
		for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryHeapCount; ++i)
		{
			const auto& heap = physicalDeviceMemoryProperties.memoryHeaps[i];
			logger.trace( "\tHeap " + std::to_string( i ) + " flags: " + vk::to_string( heap.flags ) + ", size: " + std::to_string( heap.size ) + " (" + bitToStr( heap.size ) + ")" );
		}
		logger.trace( "Memory type: " + std::to_string( physicalDeviceMemoryProperties.memoryTypeCount ) );
		for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; ++i)
		{
			const auto& type = physicalDeviceMemoryProperties.memoryTypes[i];
			logger.trace( "\tType " + std::to_string( i ) + " flags: " + vk::to_string( type.propertyFlags ) + ", heap: " + std::to_string( type.heapIndex ) );
		}
		logger.trace( "-------------------" );

		return true;
	}

	logger.critical( "Failed to find discrete GPU" );

	return false;
}

bool v3d::vulkan::Devices::initPhysicalDevice( const std::vector<vk::PhysicalDevice>& physicalDevices )
{
	for (const vk::PhysicalDevice& curPD : physicalDevices)
	{
		if (isSuitablePhysicalDevice( curPD ))
		{
			physicalDevice = curPD;

			properties = physicalDevice.getProperties();
			features = physicalDevice.getFeatures();
			memoryProperties = physicalDevice.getMemoryProperties();
			queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

			return true;
		}
	}

	return false;
}

bool v3d::vulkan::Devices::initLogicalDevice( const vk::SurfaceKHR& surface )
{
	if (!initQueueFamilyIndices( surface )) return false;

	const float queuePriority = 1.0f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo = vk::DeviceQueueCreateInfo
	(
		{},
		QFI.graphics,
		1,
		&queuePriority
	);

	std::vector<vk::ExtensionProperties> extensions = physicalDevice.enumerateDeviceExtensionProperties();

#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().logExtensions( extensions );
#endif

	std::vector<const char*> requiredExtension = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	if (!vulkan::utils::checkExtensionProperties( extensions, requiredExtension )) return false;

	vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo
	(
		{},
		1,
		&deviceQueueCreateInfo,
		0,
		nullptr,
		uint32_t( requiredExtension.size() ),
		requiredExtension.data()
	);

	logicalDevice = physicalDevice.createDevice( createInfo );

	return true;
}

bool v3d::vulkan::Devices::initQueueFamilyIndices( const vk::SurfaceKHR& surface )
{
	uint32_t i = 0;
	for (auto& queueFamilyProperty : queueFamilyProperties)
	{
		if (queueFamilyProperty.queueCount <= 0) continue;

		vk::Bool32 supportSurface = physicalDevice.getSurfaceSupportKHR( i, surface );
		if (!supportSurface) continue;

		if (queueFamilyProperty.queueFlags & (vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer))
		{
			QFI = { i,i,i,i };
			return true;
		}

		i++;
	}

	return false;
}

vk::SurfaceCapabilitiesKHR v3d::vulkan::Devices::getSurfaceCapabilitiesKHR(const vk::SurfaceKHR& surface) const
{
	return physicalDevice.getSurfaceCapabilitiesKHR(surface);
}

vk::Bool32 v3d::vulkan::Devices::getSurfaceSupportKHR(const uint32_t index, const vk::SurfaceKHR& surface) const
{
	return physicalDevice.getSurfaceSupportKHR(index, surface);
}

std::vector<vk::SurfaceFormatKHR> v3d::vulkan::Devices::getSurfaceFormatsKHR(const vk::SurfaceKHR& surface) const
{
	return physicalDevice.getSurfaceFormatsKHR(surface);
}

std::vector<vk::PresentModeKHR> v3d::vulkan::Devices::getSurfacePresentModesKHR(const vk::SurfaceKHR& surface) const
{
	return physicalDevice.getSurfacePresentModesKHR(surface);
}

vk::PhysicalDeviceProperties v3d::vulkan::Devices::getProperties() const
{
	return properties;
}

vk::PhysicalDeviceFeatures v3d::vulkan::Devices::getFeatures() const
{
	return features;
}

vk::PhysicalDeviceMemoryProperties v3d::vulkan::Devices::getMemoryProperties() const
{
	return memoryProperties;
}

std::vector<vk::ExtensionProperties> v3d::vulkan::Devices::EnumerateDeviceExtensionProperties() const
{
	return physicalDevice.enumerateDeviceExtensionProperties();
}

std::vector<vk::LayerProperties> v3d::vulkan::Devices::enumerateDeviceLayerProperties() const
{
	return physicalDevice.enumerateDeviceLayerProperties();
}

uint32_t v3d::vulkan::Devices::getGraphicsQueueFamilyIndex() const
{
	return QFI.graphics;
}

uint32_t v3d::vulkan::Devices::getComputeQueueFamilyIndex() const
{
	return QFI.compute;
}

uint32_t v3d::vulkan::Devices::getTransferQueueFamilyIndex() const
{
	return QFI.transfer;
}

uint32_t v3d::vulkan::Devices::getPresentQueueFamilyIndex() const
{
	return QFI.present;
}

uint32_t v3d::vulkan::Devices::getMemoryTypeIndex(const uint32_t memoryTypeBits, const vk::MemoryPropertyFlags memoryPropertyFlags) const
{
	const vk::PhysicalDeviceMemoryProperties memProperties = getMemoryProperties();

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((memoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags) 
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}
