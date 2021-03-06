﻿/**
*	@file CommandBuffer.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "CommandBuffer.h"

#include "CommandPool.h"
#include "Vulkan/Context.h"
#include "Vulkan/Devices/LogicalDevice.h"

V3D_NS_BEGIN
VK_NS_BEGIN

CommandBuffer::CommandBuffer( const vk::CommandBufferLevel level )
	: commandBuffer( nullptr )
	, running( false )
{
	const vk::CommandPool& cp = v3d::vulkan::Context::get()->getCommandPool()->getVKCommandPool();
	vk::CommandBufferAllocateInfo allocInfo
	(
		cp,
		level,
		1
	);

	commandBuffer = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice().allocateCommandBuffers( allocInfo ).front();
}

CommandBuffer::~CommandBuffer()
{
	const vk::CommandPool& cp = v3d::vulkan::Context::get()->getCommandPool()->getVKCommandPool();
	v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice().freeCommandBuffers( cp, commandBuffer );
}

const vk::CommandBuffer& CommandBuffer::getVKCommandBuffer() const { return commandBuffer; }

void CommandBuffer::begin( const vk::CommandBufferUsageFlags usageFlags )
{
	commandBuffer.begin( vk::CommandBufferBeginInfo( usageFlags, nullptr ) );
}

/*
void v3d::vulkan::CommandBuffer::record(const vk::Framebuffer& frameBuffer, const vk::RenderPass& renderPass, const v3d::vulkan::Swapchain& swapchain, const v3d::vulkan::Pipeline& pipeline, const vk::Buffer& vertexBuffer, const uint32_t vertexSize) const
{
	vk::ClearValue clearValue(vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f })));

	vk::RenderPassBeginInfo renderPassInfo
	(
		renderPass,
		frameBuffer,
		vk::Rect2D
		(
			vk::Offset2D(),
			swapchain.getExtent()
		),
		1,
		&clearValue
	);

	commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get());
	commandBuffer.setViewport(0, pipeline.getViewport());
	commandBuffer.setScissor(0, pipeline.getScissor());
	vk::DeviceSize offset = 0;
	commandBuffer.bindVertexBuffers(0, vertexBuffer, offset);
	commandBuffer.draw(vertexSize, 1, 0, 0);
	commandBuffer.endRenderPass();
}

void v3d::vulkan::CommandBuffer::record(const vk::Framebuffer& frameBuffer, const vk::RenderPass& renderPass, const v3d::vulkan::Swapchain& swapchain, const v3d::vulkan::Pipeline& pipeline, const vk::Buffer& vertexBuffer, const vk::Buffer& indexBuffer, const uint32_t indexSize, const vk::DescriptorSet& descriptorSet) const
{
	vk::ClearValue clearValue(vk::ClearColorValue(std::array<float, 4>({ 0.2f, 0.2f, 0.2f, 0.2f })));

	vk::RenderPassBeginInfo renderPassInfo
	(
		renderPass,
		frameBuffer,
		vk::Rect2D
		(
			vk::Offset2D(),
			swapchain.getExtent()
		),
		1,
		&clearValue
	);

	commandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get());
	commandBuffer.setViewport(0, pipeline.getViewport());
	commandBuffer.setScissor(0, pipeline.getScissor());
	vk::DeviceSize offset = 0;
	commandBuffer.bindVertexBuffers(0, vertexBuffer, offset);
	commandBuffer.bindIndexBuffer(indexBuffer, offset, vk::IndexType::eUint16);
	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline.getLayout(), 0, 1, &descriptorSet, 0, nullptr);
	commandBuffer.drawIndexed(indexSize, 1, 0, 0, 0);
	commandBuffer.endRenderPass();
}
*/

void CommandBuffer::end() { commandBuffer.end(); }

void CommandBuffer::copyBuffer( const vk::Buffer& src, const vk::Buffer& dst, const vk::DeviceSize size )
{
	const vk::BufferCopy copyRegion( 0, 0, size );
	commandBuffer.copyBuffer( src, dst, 1, &copyRegion );
}

VK_NS_END
V3D_NS_END