/**
*	@file FrameBuffer.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "FrameBuffer.h"

#include "Device.h"

v3d::vulkan::FrameBuffer::FrameBuffer()
	: framebuffer()
{}

bool v3d::vulkan::FrameBuffer::init(const vk::ImageView& imageView, const vk::Extent2D& extent, const vk::RenderPass& renderPass, const v3d::vulkan::Device& device)
{
	vk::FramebufferCreateInfo createInfo
	(
		vk::FramebufferCreateFlags(),
		renderPass,
		1,
		&imageView,
		extent.width,
		extent.height,
		1
	);

	framebuffer = device.createFrameBuffer(createInfo);

	return true;
}