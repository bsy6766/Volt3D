﻿/**
*	@file CommandBuffer.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "CommandBuffer.h"

#include "Device.h"
#include "SwapChain.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "Buffer.h"
#include "Renderer/VertexData.h"

v3d::vulkan::CommandBuffer::CommandBuffer()
	: commandBuffer()
{}

v3d::vulkan::CommandBuffer::CommandBuffer(const vk::CommandBuffer & commandBuffer)
	: commandBuffer(commandBuffer)
{}

bool v3d::vulkan::CommandBuffer::init(const v3d::vulkan::Device& device, const vk::CommandPool& commandPool)
{
	vk::CommandBufferAllocateInfo allocInfo
	(
		commandPool,
		vk::CommandBufferLevel::ePrimary,
		1
	);

	commandBuffer = device.allocateCommandBuffer(allocInfo);

	return true;
}

void v3d::vulkan::CommandBuffer::record(const vk::Framebuffer& frameBuffer, const vk::RenderPass& renderPass, const v3d::vulkan::SwapChain& swapChain, const v3d::vulkan::Pipeline& pipeline, const v3d::vulkan::Buffer& vertexBuffer, const uint32_t vertexSize) const
{
	vk::CommandBufferBeginInfo beginInfo
	(
		vk::CommandBufferUsageFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse),
		nullptr
	);

	commandBuffer.begin(beginInfo);

	vk::ClearValue clearValue(vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f })));

	vk::RenderPassBeginInfo renderPassInfo
	(
		renderPass,
		frameBuffer,
		vk::Rect2D
		(
			vk::Offset2D(),
			swapChain.getExtent2D()
		),
		1,
		&clearValue
	);

	commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get());
	commandBuffer.setViewport(0, pipeline.getViewport());
	commandBuffer.setScissor(0, pipeline.getScissor());
	vk::DeviceSize offset = 0;
	commandBuffer.bindVertexBuffers(0, vertexBuffer.get(), offset);
	commandBuffer.draw(vertexSize, 1, 0, 0);
	commandBuffer.endRenderPass();
	commandBuffer.end();
}

void v3d::vulkan::CommandBuffer::record(const vk::Framebuffer& frameBuffer, const vk::RenderPass& renderPass, const v3d::vulkan::SwapChain& swapChain, const v3d::vulkan::Pipeline& pipeline, const v3d::vulkan::Buffer& vertexBuffer, const v3d::vulkan::Buffer& indexBuffer, const uint32_t indexSize) const
{
	vk::CommandBufferBeginInfo beginInfo
	(
		vk::CommandBufferUsageFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse),
		nullptr
	);

	commandBuffer.begin(beginInfo);

	vk::ClearValue clearValue(vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f })));

	vk::RenderPassBeginInfo renderPassInfo
	(
		renderPass,
		frameBuffer,
		vk::Rect2D
		(
			vk::Offset2D(),
			swapChain.getExtent2D()
		),
		1,
		&clearValue
	);

	commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get());
	commandBuffer.setViewport(0, pipeline.getViewport());
	commandBuffer.setScissor(0, pipeline.getScissor());
	vk::DeviceSize offset = 0;
	commandBuffer.bindVertexBuffers(0, vertexBuffer.get(), offset);
	commandBuffer.bindIndexBuffer(indexBuffer.get(), offset, vk::IndexType::eUint16);
	commandBuffer.drawIndexed(indexSize, 1, 0, 0, 0);
	commandBuffer.endRenderPass();
	commandBuffer.end();
}
