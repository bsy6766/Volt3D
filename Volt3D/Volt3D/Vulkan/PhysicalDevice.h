#pragma

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

class Surface;

class PhysicalDevice
{
	friend class Vulkan;
private:
	PhysicalDevice();

	vk::PhysicalDevice physicalDevice;
	vk::PhysicalDeviceProperties physicalDeviceProperties;
	vk::PhysicalDeviceFeatures physicalDeviceFeatures;
	vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;

	bool init( vk::UniqueInstance& instance );

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( PhysicalDevice );
	DEFAULT_MOVE_CONSTRUCTORS( PhysicalDevice );
	~PhysicalDevice() {}

	vk::UniqueDevice createLogicalDevice(const vk::DeviceCreateInfo & createInfo);

	explicit operator const vk::PhysicalDevice& () const noexcept 
	{ return physicalDevice; }

	std::vector<vk::QueueFamilyProperties> getQueueFamilyProperties() const
	{ return physicalDevice.getQueueFamilyProperties(); }
	
	vk::SurfaceCapabilitiesKHR getSurfaceCapabilitiesKHR( const Surface& surface ) const;

	vk::Bool32 getSurfaceSupportKHR( const uint32_t index, const Surface& surface ) const;

	std::vector<vk::SurfaceFormatKHR> getSurfaceFormatsKHR( const Surface& surface ) const;

	std::vector<vk::PresentModeKHR> getSurfacePresentModesKHR( const Surface& surface ) const;

	vk::PhysicalDeviceProperties getProperties() const
	{ return physicalDevice.getProperties(); }

	std::vector<vk::ExtensionProperties> enumerateDeviceExtensionProperties() const
	{ return physicalDevice.enumerateDeviceExtensionProperties(); }

	std::vector<vk::LayerProperties> enumerateDeviceLayerProperties() const
	{ return physicalDevice.enumerateDeviceLayerProperties(); }
};