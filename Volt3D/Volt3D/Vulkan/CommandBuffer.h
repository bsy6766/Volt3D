/**
*	@file CommandBuffer.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_COMMAND_BUFFER_H
#define V3D_VK_COMMAND_BUFFER_H

#include <vulkan/vulkan.hpp>

#include <vector>

#include "Utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class Device;
		class SwapChain;
		class Pipeline;
		class RenderPass;
		class FrameBuffer;
		class CommandPool;

		/**
		*	@class CommandBuffer
		*	@brief A wrapper for Vulkan's commandbuffer
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL CommandBuffer
		{
		private:
			vk::UniqueCommandBuffer commandBuffer;

			bool init(const v3d::vulkan::Device& device, const v3d::vulkan::FrameBuffer& frameBuffer, const v3d::vulkan::RenderPass& renderPass, const v3d::vulkan::SwapChain& swapChain, const v3d::vulkan::Pipeline& pipeline, const v3d::vulkan::CommandPool& commandPool);

		public:
			CommandBuffer();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(CommandBuffer);
			DEFAULT_MOVE_CONSTRUCTORS(CommandBuffer);
			~CommandBuffer() {}

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueCommandBuffer, vk::CommandBuffer, commandBuffer);
		};
	}
}

#endif