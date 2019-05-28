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

#include <vector>

namespace v3d
{
	namespace vulkan
	{
		class Surface;
		class PhysicalDevice;
		class SwapChain;

		/**
		*	@class Device
		*	@breif Wrapper for Vulkan's device
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Device
		{
			friend class Context;

		private:
			vk::UniqueDevice device;
			
			bool init(v3d::vulkan::PhysicalDevice& physicalDevice);

		public:
			Device();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Device);
			DEFAULT_MOVE_CONSTRUCTORS(Device);
			~Device() {}

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueDevice, vk::Device, device);

			vk::UniqueSwapchainKHR createSwapchainKHRUnique(const vk::SwapchainCreateInfoKHR& createInfo) const;
			vk::UniqueShaderModule createShaderModuleUnique(std::vector<char> buffer) const;
			std::vector<vk::Image> getSwapchainImagesKHR(const v3d::vulkan::SwapChain& swapChain) const;
			vk::UniqueImageView createImageViewUnique(const vk::ImageViewCreateInfo& createInfo) const;
		};
	}
}

#endif