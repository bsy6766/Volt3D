/**
*	@file DeviceMemory.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_DEVICE_MEMORY_H
#define V3D_VK_DEVICE_MEMORY_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class Device;
		class PhysicalDevice;
		class Buffer;

		/**
		*	@class DeviceMemory
		*	@brief A wrapper for Vulkan's device memory
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL DeviceMemory
		{
			friend class Context;

		private:
			vk::UniqueDeviceMemory vbDeviceMemory;

			bool init(const v3d::vulkan::Device& device, const v3d::vulkan::PhysicalDevice& physicalDevice, const v3d::vulkan::Buffer& buffer, const vk::MemoryPropertyFlags memoryPropertyFlags);

		public:
			DeviceMemory();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(DeviceMemory);
			DEFAULT_MOVE_CONSTRUCTORS(DeviceMemory);
			~DeviceMemory() {}

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueDeviceMemory, vk::DeviceMemory, vbDeviceMemory);
		};
	}
}

#endif