﻿/**
*	@file Device.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Device.h"

#include "PhysicalDevice.h"
#include "SwapChain.h"
#include "CommandBuffer.h"
#include "Utils.h"

v3d::vulkan::Device::Device()
	: device()
{}

bool v3d::vulkan::Device::init(v3d::vulkan::PhysicalDevice& physicalDevice)
{
	auto graphicsQueueFamilyIndex = physicalDevice.getGraphicsQueueFamilyIndex();

	const float queuePriority = 1.0f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo = vk::DeviceQueueCreateInfo
	(
		{},
		graphicsQueueFamilyIndex,
		1,
		&queuePriority
	);

	std::vector<vk::ExtensionProperties> extensions = physicalDevice.EnumerateDeviceExtensionProperties();

#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().logExtensions(extensions);
#endif
	std::vector<const char*> requiredExtension = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	if (!vulkan::utils::checkExtensionProperties(extensions, requiredExtension)) return false;

	vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo
	(
		{},
		1,
		&deviceQueueCreateInfo,
		0,
		nullptr,
		uint32_t(requiredExtension.size()),
		requiredExtension.data()
	);

	device = physicalDevice.createDeviceUnique(createInfo);
	
	return true;
}

vk::UniqueSwapchainKHR v3d::vulkan::Device::createSwapchainKHRUnique(const vk::SwapchainCreateInfoKHR & createInfo) const
{
	return device->createSwapchainKHRUnique(createInfo);
}

vk::UniqueShaderModule v3d::vulkan::Device::createShaderModuleUnique(std::vector<char> buffer) const
{
	return device->createShaderModuleUnique(vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), buffer.size(), reinterpret_cast<const uint32_t*>(buffer.data())));
}

std::vector<vk::Image> v3d::vulkan::Device::getSwapchainImagesKHR(const v3d::vulkan::SwapChain& swapChain) const
{
	return device->getSwapchainImagesKHR(swapChain.get());
}

vk::ImageView v3d::vulkan::Device::createImageView(const vk::ImageViewCreateInfo& createInfo) const
{
	return device->createImageView(createInfo);
}

vk::PipelineLayout v3d::vulkan::Device::createPipelineLayoutUnique(const vk::PipelineLayoutCreateInfo& createInfo) const
{
	return device->createPipelineLayout(createInfo);
}

vk::UniquePipeline v3d::vulkan::Device::createPipelineUnique(const vk::GraphicsPipelineCreateInfo& createInfo) const
{
	return device->createGraphicsPipelineUnique(nullptr, createInfo);
}

vk::Framebuffer v3d::vulkan::Device::createFrameBuffer(const vk::FramebufferCreateInfo& createInfo) const
{
	return device->createFramebuffer(createInfo);
}

vk::RenderPass v3d::vulkan::Device::createRenderPass(const vk::RenderPassCreateInfo& createInfo) const
{
	return device->createRenderPass(createInfo);
}

vk::CommandPool v3d::vulkan::Device::createCommandPool(const vk::CommandPoolCreateInfo& createInfo) const
{
	return device->createCommandPool(createInfo);
}

vk::CommandBuffer v3d::vulkan::Device::allocateCommandBuffer(const vk::CommandBufferAllocateInfo& allocInfo) const
{
	return device->allocateCommandBuffers(allocInfo).front();
}

std::vector<vk::CommandBuffer> v3d::vulkan::Device::allocateCommandBuffers(const vk::CommandBufferAllocateInfo& allocInfo) const
{
	return device->allocateCommandBuffers(allocInfo);
}

vk::Semaphore v3d::vulkan::Device::createSemaphore(const vk::SemaphoreCreateInfo& createInfo) const
{
	return device->createSemaphore(createInfo);
}

vk::Fence v3d::vulkan::Device::createFence(const vk::FenceCreateInfo& createInfo) const
{
	return device->createFence(createInfo);
}

vk::ResultValue<uint32_t> v3d::vulkan::Device::acquireNextImage(const v3d::vulkan::SwapChain& swapChain, const uint64_t timeout, const vk::Semaphore& semaphore) const
{
	return device->acquireNextImageKHR(swapChain.get(), timeout, semaphore, nullptr);
}

void v3d::vulkan::Device::waitIdle() const
{
	device->waitIdle();
}

vk::Queue v3d::vulkan::Device::getQueue(const uint32_t familyIndex, const uint32_t queueIndex) const
{
	return device->getQueue(familyIndex, queueIndex);
}

void v3d::vulkan::Device::waitForFences(const vk::Fence& fence) const
{
	device->waitForFences(1, &fence, true, std::numeric_limits<uint64_t>::max());
}

void v3d::vulkan::Device::resetFences(const vk::Fence& fence) const
{
	device->resetFences(1, &fence);
}

void v3d::vulkan::Device::freeCommandBuffer(const vk::CommandPool& commandPool, const v3d::vulkan::CommandBuffer& commandBuffer) const
{
	device->freeCommandBuffers(commandPool, commandBuffer.getHandle());
}

void v3d::vulkan::Device::freeCommandBuffers(const vk::CommandPool& commandPool, const std::vector<v3d::vulkan::CommandBuffer*>& commandBuffers) const
{
	std::vector<vk::CommandBuffer> cbs;
	for (auto& cb : commandBuffers) cbs.push_back(cb->getHandle());
	device->freeCommandBuffers(commandPool, static_cast<uint32_t>(commandBuffers.size()), cbs.data());
}

vk::Buffer v3d::vulkan::Device::createBuffer(const vk::BufferCreateInfo& createInfo) const
{
	return device->createBuffer(createInfo);
}

vk::MemoryRequirements v3d::vulkan::Device::getMemoryRequirement(const vk::Buffer& buffer) const
{
	return device->getBufferMemoryRequirements(buffer);
}

vk::DeviceMemory v3d::vulkan::Device::allocateBuffer(const vk::MemoryAllocateInfo& allocInfo) const
{
	return device->allocateMemory(allocInfo);
}

void v3d::vulkan::Device::bindBufferMemory(const vk::Buffer& buffer, const vk::DeviceMemory& vbDeviceMemory) const
{
	device->bindBufferMemory(buffer, vbDeviceMemory, 0);
}

void* v3d::vulkan::Device::mapMemory(const vk::DeviceMemory& deviceMemory, const std::size_t size) const
{
	return device->mapMemory(deviceMemory, 0, static_cast<uint64_t>(size));
}

void v3d::vulkan::Device::unMapMemory(const vk::DeviceMemory& deviceMemory) const
{
	device->unmapMemory(deviceMemory);
}

vk::DescriptorSetLayout v3d::vulkan::Device::createDescriptorSetLayout(const vk::DescriptorSetLayoutCreateInfo& createInfo) const
{
	return device->createDescriptorSetLayout(createInfo);
}

vk::DescriptorPool v3d::vulkan::Device::createDescriptorPool(const vk::DescriptorPoolCreateInfo createInfo) const
{
	return device->createDescriptorPool(createInfo);
}
