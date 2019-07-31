/**
*	@file SwapChain.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_SWAPCHAIN_H
#define V3D_VK_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class PhysicalDevice;
		class Surface;

		/**
		*	@class SwapChain
		*	@brief Wrapper for Vulkan's swapchain.
		*
		*	@group Vulkan
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

			bool init( const vk::PhysicalDevice& physicalDevice, const vk::Device& logicalDevice, const vk::SurfaceKHR& surface, const glm::ivec2& frameBufferSize );

			vk::SurfaceFormatKHR selectSurfaceFormat( const std::vector<vk::SurfaceFormatKHR>& surfaceFormats ) const;
			vk::Extent2D selectExtent( const vk::SurfaceCapabilitiesKHR& surfaceCapabilities, const glm::ivec2& frameBufferSize ) const;
			vk::PresentModeKHR selectPresentMode( const std::vector<vk::PresentModeKHR>& presentModes ) const;

		public:
			SwapChain();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( SwapChain );
			DEFAULT_MOVE_CONSTRUCTORS( SwapChain );
			~SwapChain() {};

			UNIQUE_TO_CPP_VK_HANDLE( vk::UniqueSwapchainKHR, vk::SwapchainKHR, swapChain );

			const vk::Format& getFormat() const;
			const vk::Extent2D& getExtent2D() const;
		};
	}
}

#endif