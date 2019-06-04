/**
*	@file CommandPool.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "CommandPool.h"

#include "PhysicalDevice.h"
#include "Device.h"
#include "SwapChain.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "FrameBuffer.h"

v3d::vulkan::CommandPool::CommandPool()
	: commandPool()
	, commandBuffers()
{}

bool v3d::vulkan::CommandPool::init(const v3d::vulkan::PhysicalDevice& physicalDevice, const v3d::vulkan::Device& device, const v3d::vulkan::FrameBuffer& frameBuffer, const v3d::vulkan::RenderPass& renderPass, const v3d::vulkan::SwapChain& swapChain, const v3d::vulkan::Pipeline& pipeline)
{
	uint32_t graphicsFamilyIndex = physicalDevice.getGraphicsQueueFamilyIndex();

	vk::CommandPoolCreateInfo createInfo
	(
		vk::CommandPoolCreateFlags(),
		graphicsFamilyIndex
	);

	commandPool = device.createCommandPool(createInfo);

	const auto& frameBuffers = frameBuffer.getFrameBuffers();
	const std::size_t fbSize = frameBuffers.size();
	
	vk::CommandBufferAllocateInfo allocInfo
	(
		commandPool.get(),
		vk::CommandBufferLevel::ePrimary,
		static_cast<uint32_t>(fbSize)
	);

	commandBuffers = device.allocateCommandBuffers(allocInfo);

	for (std::size_t i = 0; i < fbSize; i++)
	{
		vk::CommandBufferBeginInfo beginInfo
		(
			vk::CommandBufferUsageFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse),
			nullptr
		);

		vk::CommandBuffer& cb = commandBuffers[i].get();
		cb.begin(beginInfo);
		
		vk::ClearValue clearValue(vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f })));

		vk::RenderPassBeginInfo renderPassInfo
		(
			renderPass.get(),
			frameBuffers[i].get(),
			vk::Rect2D
			(
				vk::Offset2D(),
				swapChain.getExtent2D()
			),
			1,
			&clearValue
		);

		cb.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
		cb.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get());
		cb.setViewport(0, pipeline.getViewport());
		cb.setScissor(0, pipeline.getScissor());
		cb.draw(3, 1, 0, 0);
		cb.endRenderPass();
		cb.end();
	}

	return true;
}
