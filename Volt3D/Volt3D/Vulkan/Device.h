﻿/**
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
		class Semaphore;
		class Fence;
		class CommandPool;
		class CommandBuffer;

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
			vk::PipelineLayout createPipelineLayoutUnique(const vk::PipelineLayoutCreateInfo& createInfo) const;
			vk::UniquePipeline createPipelineUnique(const vk::GraphicsPipelineCreateInfo& createInfo) const;
			vk::Framebuffer createFrameBuffer(const vk::FramebufferCreateInfo& createInfo) const;
			vk::RenderPass createRenderPass(const vk::RenderPassCreateInfo& createInfo) const;
			vk::CommandPool createCommandPool(const vk::CommandPoolCreateInfo& createInfo) const;
			vk::CommandBuffer allocateCommandBuffer(const vk::CommandBufferAllocateInfo& allocInfo) const;
			std::vector<vk::CommandBuffer> allocateCommandBuffers(const vk::CommandBufferAllocateInfo& allocInfo) const;
			vk::UniqueSemaphore createSemaphore(const vk::SemaphoreCreateInfo& createInfo) const;
			vk::UniqueFence createFence(const vk::FenceCreateInfo& createInfo) const;
			vk::ResultValue<uint32_t> acquireNextImage(const v3d::vulkan::SwapChain& swapChain, const uint64_t timeout, const v3d::vulkan::Semaphore& semaphore) const;
			void waitIdle() const;
			vk::Queue getQueue(const uint32_t familyIndex, const uint32_t queueIndex = 0) const;
			void waitForFences(const v3d::vulkan::Fence& fence) const;
			void resetFences(const v3d::vulkan::Fence& fence) const;
			void freeCommandBuffer(const vk::CommandPool& commandPool, const v3d::vulkan::CommandBuffer& commandBuffer) const;
			void freeCommandBuffers(const vk::CommandPool& commandPool, const std::vector<v3d::vulkan::CommandBuffer*>& commandBuffers) const;
			vk::Buffer createBuffer(const vk::BufferCreateInfo& createInfo) const;
			vk::MemoryRequirements getMemoryRequirement(const vk::Buffer& buffer) const;
			vk::DeviceMemory allocateBuffer(const vk::MemoryAllocateInfo& allocInfo) const;
			void bindBufferMemory(const vk::Buffer& buffer, const vk::DeviceMemory& vbDeviceMemory) const;
			void* mapMemory(const vk::DeviceMemory& deviceMemory, const std::size_t size) const;
			void unMapMemory(const vk::DeviceMemory& deviceMemory) const;
			vk::DescriptorSetLayout createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& createInfo) const;
		};
	}
}

#endif