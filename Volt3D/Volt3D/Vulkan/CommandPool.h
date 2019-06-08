/**
*	@file CommandPool.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_COMMAND_POOL_H
#define V3D_VK_COMMAND_POOL_H

#include <vulkan/vulkan.hpp>

#include <vector>

#include "Utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class PhysicalDevice;
		class Device;
		class SwapChain;
		class Pipeline;
		class RenderPass;
		class FrameBuffer;

		/**
		*	@class CommandPool
		*	@brief A wrapper for Vulkan's command pool.
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL CommandPool
		{
			friend class Context;
			friend class Device;

		private:
			vk::UniqueCommandPool commandPool;
			std::vector<vk::CommandBuffer> commandBuffers;

			bool init(const v3d::vulkan::PhysicalDevice& physicalDevice, const v3d::vulkan::Device& device);
			bool initCommandBuffers(const v3d::vulkan::Device& device, const v3d::vulkan::FrameBuffer& frameBuffer, const v3d::vulkan::RenderPass& renderPass, const v3d::vulkan::SwapChain& swapChain, const v3d::vulkan::Pipeline& pipeline);
			void clearCommandBuffers();

		public:
			CommandPool();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(CommandPool);
			DEFAULT_MOVE_CONSTRUCTORS(CommandPool);
			~CommandPool() {}

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueCommandPool, vk::CommandPool, commandPool);

			const vk::CommandBuffer& getBufferAt(const uint32_t index) const;
			std::size_t getBufferSize() const;
			const vk::CommandBuffer* getBufferData() const;
		};
	}
}

#endif