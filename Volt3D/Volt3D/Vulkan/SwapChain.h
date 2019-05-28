/**
*	@file Surface.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_SWAPCHAIN_H
#define V3D_VK_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class PhysicalDevice;
		class Device;
		class Surface;

		/**
		*	@class SwapChain
		*	@brief Wrapper for Vulkan's swapchain.
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL SwapChain
		{
		private:
			vk::UniqueSwapchainKHR swapChain;
			std::vector<vk::UniqueImageView> imageViews;

		public:
			SwapChain(vk::UniqueSwapchainKHR&& swapChain);
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(SwapChain);
			DEFAULT_MOVE_CONSTRUCTORS(SwapChain);
			~SwapChain() {};

			CLASS_TO_VULKAN_HANDLE(vk::UniqueSwapchainKHR, swapChain);
		};
	}
}

#endif