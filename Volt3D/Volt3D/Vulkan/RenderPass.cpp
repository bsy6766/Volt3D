/**
*	@file RenderPass.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "RenderPass.h"

#include "Device.h"
#include "SwapChain.h"

v3d::vulkan::RenderPass::RenderPass()
	: renderPass()
{}

bool v3d::vulkan::RenderPass::init(const v3d::vulkan::Device& device, const v3d::vulkan::SwapChain& swapChain)
{
	vk::AttachmentDescription attachmentDescriptions
	(
		vk::AttachmentDescriptionFlags(),
		swapChain.getFormat(),
		vk::SampleCountFlagBits::e1,
		vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eStore,
		vk::AttachmentLoadOp::eDontCare,
		vk::AttachmentStoreOp::eDontCare,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::ePresentSrcKHR
	);
	
	vk::AttachmentReference colorAttachment
	(
		0,
		vk::ImageLayout::eColorAttachmentOptimal
	);

	// @note visit later
	//vk::AttachmentReference depthAttachment
	//(
	//	1, 
	//	vk::ImageLayout::eDepthStencilAttachmentOptimal
	//);
	
	vk::SubpassDescription subpassDescription
	(
		vk::SubpassDescriptionFlags(), 
		vk::PipelineBindPoint::eGraphics, 
		0, 
		nullptr, 
		1, 
		&colorAttachment//,
		//nullptr, 
		//(depthFormat != vk::Format::eUndefined) ? &depthAttachment : nullptr
	);
	
	vk::RenderPassCreateInfo createInfo
	(
		vk::RenderPassCreateFlags(), 
		1,
		&attachmentDescriptions,
		1, 
		&subpassDescription
	);
	
	renderPass = device.get().createRenderPassUnique(createInfo);

	return true;
}
