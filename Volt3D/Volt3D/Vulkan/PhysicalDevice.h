/**
*	@file PhysicalDevice.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_PHYSICAL_DEVICE_H
#define V3D_VK_PHYSICAL_DEVICE_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class Surface;

		/**
		*	@class PhysicalDevice
		*	@brief A wrapper of vulkan's physical device (GPU)
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class PhysicalDevice
		{
			friend class Context;

		private:
			vk::PhysicalDevice physicalDevice;
			vk::PhysicalDeviceProperties physicalDeviceProperties;
			vk::PhysicalDeviceFeatures physicalDeviceFeatures;
			vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;

			//bool init(vk::UniqueInstance& instance);

			static bool isSuitable(const vk::PhysicalDevice& physicalDevice);

		public:
			PhysicalDevice(vk::PhysicalDevice&& physicalDevice);
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(PhysicalDevice);
			DEFAULT_MOVE_CONSTRUCTORS(PhysicalDevice);
			~PhysicalDevice() {}

			CLASS_TO_VULKAN_HANDLE(vk::PhysicalDevice, physicalDevice);

			/*
			vk::UniqueDevice createLogicalDevice(const vk::DeviceCreateInfo& createInfo);

			std::vector<vk::QueueFamilyProperties> getQueueFamilyProperties() const
			{
				return physicalDevice.getQueueFamilyProperties();
			}

			vk::SurfaceCapabilitiesKHR getSurfaceCapabilitiesKHR(const Surface& surface) const;

			vk::Bool32 getSurfaceSupportKHR(const uint32_t index, const Surface& surface) const;

			std::vector<vk::SurfaceFormatKHR> getSurfaceFormatsKHR(const Surface& surface) const;

			std::vector<vk::PresentModeKHR> getSurfacePresentModesKHR(const Surface& surface) const;

			vk::PhysicalDeviceProperties getProperties() const
			{
				return physicalDevice.getProperties();
			}

			std::vector<vk::ExtensionProperties> enumerateDeviceExtensionProperties() const
			{
				return physicalDevice.enumerateDeviceExtensionProperties();
			}

			std::vector<vk::LayerProperties> enumerateDeviceLayerProperties() const
			{
				return physicalDevice.enumerateDeviceLayerProperties();
			}
			*/
		};
	}
}

#endif