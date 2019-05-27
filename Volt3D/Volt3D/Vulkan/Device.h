/**
*	@file Device.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_DEVICE_H
#define V3D_VK_DEVICE_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class Surface;
		class PhysicalDevice;

		class Device
		{
			friend class Vulkan;
		private:
			vk::UniqueDevice device;
			
		public:
			Device(vk::UniqueDevice&& device);
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Device);
			DEFAULT_MOVE_CONSTRUCTORS(Device);
			~Device() {}
		};
	}
}

#endif