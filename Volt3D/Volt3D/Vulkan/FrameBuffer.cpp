/**
*	@file FrameBuffer.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "FrameBuffer.h"

#include "SwapChain.h"
#include "RenderPass.h"
#include "Device.h"

v3d::vulkan::FrameBuffer::FrameBuffer()
	: frameBuffers()
{}

bool v3d::vulkan::FrameBuffer::init(const v3d::vulkan::SwapChain& swapChain, const v3d::vulkan::RenderPass& renderPass, const v3d::vulkan::Device& device)
{
	const auto& imageViews = swapChain.getImageViews();
	const std::size_t size = imageViews.size();
	const auto& extent = swapChain.getExtent2D();

	frameBuffers.resize(size);

	for (std::size_t i = 0; i < size; i++)
	{
		vk::FramebufferCreateInfo createInfo
		(
			vk::FramebufferCreateFlags(),
			renderPass.get(),
			1,
			&imageViews[i].get(),
			extent.width,
			extent.height,
			1
		);

		frameBuffers[i] = device.createFrameBuffer(createInfo);
	}

	return true;
}

const std::vector<vk::UniqueFramebuffer>& v3d::vulkan::FrameBuffer::getFrameBuffers() const
{
	return frameBuffers;
}

std::size_t v3d::vulkan::FrameBuffer::size() const
{
	return frameBuffers.size();
}

const vk::UniqueFramebuffer& v3d::vulkan::FrameBuffer::getFrameBuffer(const std::size_t index) const
{
	return frameBuffers.at(index);
}
