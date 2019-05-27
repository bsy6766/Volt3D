﻿/**
*	@file PhysicalDevice.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_PHYSICAL_DEVICE_H
#define V3D_VK_PHYSICAL_DEVICE_H

#include <vulkan/vulkan.hpp>

#include <optional>
#include <vector>

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
			//uint32_t graphicsQueueFamilyIndex;
			//uint32_t presentQueueFamilyIndex;
			
			static bool isSuitable(const vk::PhysicalDevice& physicalDevice);

		public:
			PhysicalDevice(vk::PhysicalDevice&& physicalDevice);
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(PhysicalDevice);
			DEFAULT_MOVE_CONSTRUCTORS(PhysicalDevice);
			~PhysicalDevice() {}

			CLASS_TO_VULKAN_HANDLE(vk::PhysicalDevice, physicalDevice);
			
			std::optional<uint32_t> getGraphicsQueueFamilyIndex();
			std::optional<uint32_t> getPresentsQueueFamilyIndex(v3d::vulkan::Surface& surface);

			inline vk::UniqueDevice createDeviceUnique(vk::DeviceCreateInfo& createInfo) const;
			inline std::vector<vk::QueueFamilyProperties> getQueueFamilyProperties() const;
			inline vk::SurfaceCapabilitiesKHR getSurfaceCapabilitiesKHR(const Surface& surface) const;
			inline vk::Bool32 getSurfaceSupportKHR(const uint32_t index, const Surface& surface) const;
			inline std::vector<vk::SurfaceFormatKHR> getSurfaceFormatsKHR(const Surface& surface) const;
			inline std::vector<vk::PresentModeKHR> getSurfacePresentModesKHR(const Surface& surface) const;
			inline vk::PhysicalDeviceProperties getProperties() const;
			inline vk::PhysicalDeviceFeatures getFeatures() const;
			inline vk::PhysicalDeviceMemoryProperties getMemoryProperties() const;
			inline std::vector<vk::ExtensionProperties> enumerateDeviceExtensionProperties() const;
			inline std::vector<vk::LayerProperties> enumerateDeviceLayerProperties() const;
			//inline uint32_t getGraphicsQueueFamilyIndex() const	{ return graphicsQueueFamilyIndex; }
			//inline uint32_t getPresentQueueFamilyIndex() const	{ return presentQueueFamilyIndex; }
		};
	}
}

#endif