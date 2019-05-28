/**
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
		class VOLT3D_DLL PhysicalDevice
		{
			friend class Context;

		private:
			vk::PhysicalDevice physicalDevice;
			std::optional<uint32_t> graphicsQueueFamilyIndex;
			std::optional<uint32_t> presentQueueFamilyIndex;
			
			static bool isSuitable(const vk::PhysicalDevice& physicalDevice);

			std::optional<uint32_t> initGraphicsQueueFamilyIndex();
			std::optional<uint32_t> initPresentsQueueFamilyIndex(v3d::vulkan::Surface& surface);

		public:
			PhysicalDevice(vk::PhysicalDevice&& physicalDevice, Surface& surface);
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(PhysicalDevice);
			DEFAULT_MOVE_CONSTRUCTORS(PhysicalDevice);
			~PhysicalDevice() {}

			CLASS_TO_VULKAN_HANDLE(vk::PhysicalDevice, physicalDevice);
			
			vk::UniqueDevice createDeviceUnique(vk::DeviceCreateInfo& createInfo) const;
			std::vector<vk::QueueFamilyProperties> getQueueFamilyProperties() const;
			vk::SurfaceCapabilitiesKHR getSurfaceCapabilitiesKHR(const Surface& surface) const;
			vk::Bool32 getSurfaceSupportKHR(const uint32_t index, const Surface& surface) const;
			std::vector<vk::SurfaceFormatKHR> getSurfaceFormatsKHR(const Surface& surface) const;
			std::vector<vk::PresentModeKHR> getSurfacePresentModesKHR(const Surface& surface) const;
			vk::PhysicalDeviceProperties getProperties() const;
			vk::PhysicalDeviceFeatures getFeatures() const;
			vk::PhysicalDeviceMemoryProperties getMemoryProperties() const;
			std::vector<vk::ExtensionProperties> EnumerateDeviceExtensionProperties() const;
			std::vector<vk::LayerProperties> enumerateDeviceLayerProperties() const;
			std::optional<uint32_t> getGraphicsQueueFamilyIndex() const;
			std::optional<uint32_t> getPresentQueueFamilyIndex() const;
		};
	}
}

#endif