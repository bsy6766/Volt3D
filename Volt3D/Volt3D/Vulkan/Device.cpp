/**
*	@file Device.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Device.h"

#include "PhysicalDevice.h"
#include "SwapChain.h"
#include "Semaphore.h"
#include "Fence.h"
#include "CommandPool.h"
#include "Buffer.h"
#include "DeviceMemory.h"
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

vk::UniqueImageView v3d::vulkan::Device::createImageViewUnique(const vk::ImageViewCreateInfo& createInfo) const
{
	return device->createImageViewUnique(createInfo);
}

vk::UniquePipelineLayout v3d::vulkan::Device::createPipelineLayoutUnique(const vk::PipelineLayoutCreateInfo& createInfo) const
{
	return device->createPipelineLayoutUnique(createInfo);
}

vk::UniquePipeline v3d::vulkan::Device::createPipelineUnique(const vk::GraphicsPipelineCreateInfo& createInfo) const
{
	return device->createGraphicsPipelineUnique(nullptr, createInfo);
}

vk::UniqueFramebuffer v3d::vulkan::Device::createFrameBuffer(const vk::FramebufferCreateInfo& createInfo) const
{
	return device->createFramebufferUnique(createInfo);
}

vk::UniqueCommandPool v3d::vulkan::Device::createCommandPool(const vk::CommandPoolCreateInfo& createInfo) const
{
	return device->createCommandPoolUnique(createInfo);
}

std::vector<vk::CommandBuffer> v3d::vulkan::Device::allocateCommandBuffers(const vk::CommandBufferAllocateInfo& allocInfo) const
{
	return device->allocateCommandBuffers(allocInfo);
}

vk::UniqueSemaphore v3d::vulkan::Device::createSemaphore(const vk::SemaphoreCreateInfo& createInfo) const
{
	return device->createSemaphoreUnique(createInfo);
}

vk::UniqueFence v3d::vulkan::Device::createFence(const vk::FenceCreateInfo& createInfo) const
{
	return device->createFenceUnique(createInfo);
}

vk::ResultValue<uint32_t> v3d::vulkan::Device::acquireNextImage(const v3d::vulkan::SwapChain& swapChain, const uint64_t timeout, const v3d::vulkan::Semaphore& semaphore) const
{
	return device->acquireNextImageKHR(swapChain.get(), timeout, semaphore.get(), nullptr);
}

void v3d::vulkan::Device::waitIdle() const
{
	device->waitIdle();
}

vk::Queue v3d::vulkan::Device::getQueue(const uint32_t familyIndex, const uint32_t queueIndex) const
{
	return device->getQueue(familyIndex, queueIndex);
}

void v3d::vulkan::Device::waitForFences(const v3d::vulkan::Fence& fence) const
{
	device->waitForFences(1, &fence.get(), true, std::numeric_limits<uint64_t>::max());
}

void v3d::vulkan::Device::resetFences(const v3d::vulkan::Fence& fence) const
{
	device->resetFences(1, &fence.get());
}

void v3d::vulkan::Device::freeCommandBuffers(const v3d::vulkan::CommandPool& commandPool) const
{
	device->freeCommandBuffers(commandPool.get(), static_cast<uint32_t>(commandPool.getBufferSize()), commandPool.getBufferData());
}

vk::UniqueBuffer v3d::vulkan::Device::createBuffer(const vk::BufferCreateInfo& createInfo) const
{
	return device->createBufferUnique(createInfo);
}

vk::MemoryRequirements v3d::vulkan::Device::getMemoryRequirement(const v3d::vulkan::Buffer& buffer) const
{
	return device->getBufferMemoryRequirements(buffer.get());
}

vk::UniqueDeviceMemory v3d::vulkan::Device::allocateBuffer(const vk::MemoryAllocateInfo& allocInfo) const
{
	return device->allocateMemoryUnique(allocInfo);
}

void v3d::vulkan::Device::bindBufferMemory(const v3d::vulkan::Buffer& buffer, const v3d::vulkan::DeviceMemory& deviceMemory) const
{
	device->bindBufferMemory(buffer.get(), deviceMemory.get(), 0);
}
