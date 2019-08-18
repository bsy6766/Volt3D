/**
*	@file Framebuffers.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Framebuffers.h"

#include "Vulkan/Devices/LogicalDevice.h"
#include "Vulkan/Swapchain/Swapchain.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Framebuffers::Framebuffers( const v3d::vulkan::Swapchain& swapchain, const vk::RenderPass& renderPass )
	: framebuffers()
{
	const std::vector<vk::ImageView>& imageViews = swapchain.getImageViews();
	const vk::Extent2D& extent = swapchain.getExtent();

	const std::size_t size = imageViews.size();
	framebuffers.resize( size );

	const vk::Device& logicalDevice = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice();

	for (std::size_t i = 0; i < size; i++)
	{
		vk::FramebufferCreateInfo createInfo
		(
			vk::FramebufferCreateFlags(),
			renderPass,
			1,
			&imageViews[i],
			extent.width,
			extent.height,
			1
		);

		framebuffers[i] = logicalDevice.createFramebuffer( createInfo );
	}
}

Framebuffers::~Framebuffers()
{
	const vk::Device& logicalDevice = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice();
	for (auto& framebuffer : framebuffers) logicalDevice.destroyFramebuffer( framebuffer );
}

const vk::Framebuffer& Framebuffers::operator[]( const std::size_t index )
{
	return framebuffers[index];
}

const std::size_t Framebuffers::size() const { return framebuffers.size(); }

const std::vector<vk::Framebuffer>& Framebuffers::getFramebuffers() const
{
	return framebuffers;
}

VK_NS_END
V3D_NS_END