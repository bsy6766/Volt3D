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
		class Swapchain;
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
			friend class Context;

		private:
			CommandBuffer() = delete;
						
			bool running;
			vk::CommandBuffer commandBuffer;

		public:
			CommandBuffer( const vk::CommandBufferLevel level );
			~CommandBuffer();

			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( CommandBuffer );
			DEFAULT_MOVE_CONSTRUCTORS( CommandBuffer );

			/** Get Vulkan command buffer */
			const vk::CommandBuffer& getVKCommandBuffer() const;

			/**
			*	Begin command buffer.
			*	@param usageFlags A vulkan command buffer usage flag to begin with
			*/
			void begin( const vk::CommandBufferUsageFlags usageFlags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit );

			//void record( const vk::Framebuffer& frameBuffer, const vk::RenderPass& renderPass, const v3d::vulkan::Swapchain& swapchain, const v3d::vulkan::Pipeline& pipeline, const vk::Buffer& vertexBuffer, const uint32_t vertexSize ) const;
			//void record
			//( const vk::Framebuffer& frameBuffer,
			//	const vk::RenderPass& renderPass,
			//	const v3d::vulkan::Swapchain& swapchain,
			//	const v3d::vulkan::Pipeline& pipeline,
			//	const vk::Buffer& vertexBuffer,
			//	const vk::Buffer& indexBuffer,
			//	const uint32_t indexSize,
			//	const vk::DescriptorSet& descriptorSet
			//) const;

			/** End command buffer */
			void end();

			void copyBuffer( const vk::Buffer& src, const vk::Buffer& dst, const vk::DeviceSize size );
		};
	}
}

#endif