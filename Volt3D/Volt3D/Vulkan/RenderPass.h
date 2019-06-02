/**
*	@file PhysicalDevice.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_RENDER_PASS_H
#define V3D_VK_RENDER_PASS_H

#include <vulkan/vulkan.hpp>

namespace v3d
{
	namespace vulkan
	{
		class Device;
		class SwapChain;

		/**
		*	@class RenderPass
		*	@brief Wrapper for Vulkan's RenderPass
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class RenderPass
		{
			friend class Context;
		private:
			vk::UniqueRenderPass renderPass;

			bool init(const v3d::vulkan::Device& device, const v3d::vulkan::SwapChain& swapChain);

		public:
			RenderPass();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(RenderPass);
			DEFAULT_MOVE_CONSTRUCTORS(RenderPass);
			~RenderPass() {}

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueRenderPass, vk::RenderPass, renderPass);
		};
	}
}

#endif