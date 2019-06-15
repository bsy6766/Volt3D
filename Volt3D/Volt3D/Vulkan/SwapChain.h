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
			vk::SurfaceFormatKHR surfaceFormat;
			vk::Extent2D extent;

			bool init(const v3d::vulkan::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::SurfaceKHR& surface, const v3d::glfw::Window& window);

			vk::SurfaceFormatKHR selectSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& surfaceFormats) const;
			vk::Extent2D selectExtent(const vk::SurfaceCapabilitiesKHR& surfaceCapabilities, const v3d::glfw::Window& window) const;
			vk::PresentModeKHR selectPresentMode(const std::vector<vk::PresentModeKHR>& presentModes) const;

		public:
			SwapChain();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(SwapChain);
			DEFAULT_MOVE_CONSTRUCTORS(SwapChain);
			~SwapChain() {};

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueSwapchainKHR, vk::SwapchainKHR, swapChain);

			const vk::Format& getFormat() const;
			const vk::Extent2D& getExtent2D() const;
		};
	}
}

#endif