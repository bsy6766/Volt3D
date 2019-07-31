﻿/**
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
		class SwapChain;
		class Pipeline;
		class CommandPool;

		/**
		*	@class CommandBuffer
		*	@brief A wrapper for Vulkan's commandbuffer
		*
		*	@group Vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL CommandBuffer
		{
		private:
			bool init(const vk::Device& device, const vk::CommandPool& commandPool);

		public:
			CommandBuffer();
			CommandBuffer(const vk::CommandBuffer& commandBuffer);
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(CommandBuffer);
			DEFAULT_MOVE_CONSTRUCTORS(CommandBuffer);
			~CommandBuffer() {}

			vk::CommandBuffer commandBuffer;

			CLASS_TO_VK_HANDLE(vk::CommandBuffer, commandBuffer);

			void begin( const vk::CommandBufferUsageFlags usageFlags );
			void record(const vk::Framebuffer& frameBuffer, const vk::RenderPass& renderPass, const v3d::vulkan::SwapChain& swapChain, const v3d::vulkan::Pipeline& pipeline, const vk::Buffer& vertexBuffer, const uint32_t vertexSize) const;
			void record
			(	const vk::Framebuffer& frameBuffer, 
				const vk::RenderPass& renderPass, 
				const v3d::vulkan::SwapChain& swapChain, 
				const v3d::vulkan::Pipeline& pipeline, 
				const vk::Buffer& vertexBuffer, 
				const vk::Buffer& indexBuffer, 
				const uint32_t indexSize,
				const vk::DescriptorSet& descriptorSet
			) const;
			void end();
			void copyBuffer( const vk::Buffer& src, const vk::Buffer& dst, const vk::DeviceSize size );
		};
	}
}

#endif