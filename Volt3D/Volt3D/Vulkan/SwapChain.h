/**
*	@file SwapChain.h
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
	namespace glfw { class Window; }
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
			friend class Context;

		private:
			vk::UniqueSwapchainKHR swapChain;
			vk::Format format;
			vk::Extent2D extent;
			std::vector<vk::Image> swapChainImages;
			std::vector<vk::UniqueImageView> imageViews;

			bool init(v3d::vulkan::PhysicalDevice& physicalDevice, v3d::vulkan::Device& device, v3d::vulkan::Surface& surface);

		public:
			SwapChain();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(SwapChain);
			DEFAULT_MOVE_CONSTRUCTORS(SwapChain);
			~SwapChain() {};

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueSwapchainKHR, vk::SwapchainKHR, swapChain);

			const vk::Format& getFormat() const;
			const vk::Extent2D& getExtent2D() const;
			const std::size_t getImageViewsSize() const;
			const std::vector<vk::UniqueImageView>& getImageViews() const;
		};
	}
}

#endif