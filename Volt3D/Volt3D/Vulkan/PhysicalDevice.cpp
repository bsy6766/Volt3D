#include <PreCompiled.h>

#include "Surface.h"
#include "PhysicalDevice.h"

#include <sstream>

PhysicalDevice::PhysicalDevice()
	: physicalDevice(nullptr)
	, physicalDeviceProperties()
	, physicalDeviceFeatures()
	, physicalDeviceMemoryProperties()
{}

bool PhysicalDevice::init( vk::UniqueInstance & instance )
{
	auto& logger = Logger::getInstance();

	auto physicalDevices = instance.get().enumeratePhysicalDevices();

	if( physicalDevices.empty() )
	{
		logger.critical( "Failed to find GPU that supports Context" );
		return false;
	}

	logger.trace( "Total " + std::to_string( physicalDevices.size() ) + " physical devices found" );

	auto bitToStr = []( uint64_t size ) -> std::string
	{
		const std::vector<std::string> SUFFIXES{ { "B", "KB", "MB", "GB", "TB", "PB" } };
		size_t suffixIndex = 0;
		while( suffixIndex < SUFFIXES.size() - 1 && size > 1024 ) {
			size >>= 10;
			++suffixIndex;
		}

		std::stringstream buffer;
		buffer << size << " " << SUFFIXES[suffixIndex];
		return buffer.str();
	};

	for( auto& pd : physicalDevices )
	{
		vk::PhysicalDeviceProperties properties = pd.getProperties();

		if( properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu )
		{
			logger.info( "Found discrete GPU" );

			physicalDevice = pd;
			physicalDeviceProperties = properties;
			physicalDeviceFeatures = pd.getFeatures();
			physicalDeviceMemoryProperties = pd.getMemoryProperties();

			logger.trace( "GPU information ---" );
			logger.trace( "Driver Version: " + std::to_string( physicalDeviceProperties.driverVersion ) );
			logger.trace( "Device Name: " + std::string( physicalDeviceProperties.deviceName ) );
			logger.trace( "Device Type: VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU" );
			const int major = (physicalDeviceProperties.apiVersion >> 22) & 0x3FF;
			const int minor = (physicalDeviceProperties.apiVersion >> 12) & 0x3FF;
			const int version = (physicalDeviceProperties.apiVersion & 0xFFF);
			logger.trace( "API Version: " + std::to_string( major ) + "." + std::to_string( minor ) + "." + std::to_string( version ) );
			logger.trace( "Memory heaps: " + std::to_string( physicalDeviceMemoryProperties.memoryHeapCount ) );
			for( uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryHeapCount; ++i )
			{
				const auto& heap = physicalDeviceMemoryProperties.memoryHeaps[i];
				logger.trace( "\tHeap " + std::to_string( i ) + " flags: " + vk::to_string( heap.flags ) + ", size: " + std::to_string( heap.size ) + " (" + bitToStr( heap.size ) + ")" );
			}
			logger.trace( "Memory type: " + std::to_string( physicalDeviceMemoryProperties.memoryTypeCount ) );
			for( uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; ++i )
			{
				const auto& type = physicalDeviceMemoryProperties.memoryTypes[i];
				logger.trace( "\tType " + std::to_string( i ) + " flags: " + vk::to_string( type.propertyFlags ) + ", heap: " + std::to_string( type.heapIndex ) );
			}
			logger.trace( "-------------------" );

			return true;
		}
	}

	logger.critical( "Failed to find discrete GPU" );

	return false;
}

vk::UniqueDevice PhysicalDevice::createLogicalDevice( const vk::DeviceCreateInfo& createInfo )
{
	return std::move( physicalDevice.createDeviceUnique( createInfo ) );
}

vk::SurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilitiesKHR( const Surface& surface ) const
{
	return physicalDevice.getSurfaceCapabilitiesKHR( reinterpret_cast<const vk::UniqueSurfaceKHR&>(surface).get() );
}

vk::Bool32 PhysicalDevice::getSurfaceSupportKHR( const uint32_t index, const Surface& surface ) const
{
	return physicalDevice.getSurfaceSupportKHR( index, reinterpret_cast<const vk::UniqueSurfaceKHR&>(surface).get() );
}

std::vector<vk::SurfaceFormatKHR> PhysicalDevice::getSurfaceFormatsKHR( const Surface& surface ) const
{
	return physicalDevice.getSurfaceFormatsKHR( reinterpret_cast<const vk::UniqueSurfaceKHR&>(surface).get() );
}

std::vector<vk::PresentModeKHR> PhysicalDevice::getSurfacePresentModesKHR( const Surface& surface ) const
{
	return physicalDevice.getSurfacePresentModesKHR( reinterpret_cast<const vk::UniqueSurfaceKHR&>(surface).get() );
}