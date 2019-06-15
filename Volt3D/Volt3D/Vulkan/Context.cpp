/**
*	@file Context.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Context.h"

#include "Core/Window.h"
#include "Instance.h"
#include "DebugReportCallback.h"
#include "DebugUtilsMessenger.h"
#include "PhysicalDevice.h"
#include "Device.h"
#include "SwapChain.h"
#include "ShaderModule.h"
#include "Pipeline.h"
#include "Queue.h"
#include "CommandBuffer.h"
#include "Utils.h"
#include "Config/BuildConfig.h"

#include "Renderer/VertexData.cpp"

v3d::vulkan::Context::Context(const v3d::glfw::Window& window)
	: instance(nullptr)
	, validationLayerEnabled(false)
	, debugReportCallback(nullptr)
	, debugUtilsMessenger(nullptr)
	, surface(nullptr)
	, physicalDevice(nullptr)
	, device(nullptr)
	, swapChain(nullptr)
	, renderPass(nullptr)
	, pipeline(nullptr)
	, framebuffers()
	, commandPool(nullptr)
	, imageAvailableSemaphores()
	, renderFinishedSemaphores()
	, frameFences()
	, graphicsQueue(nullptr)
	, presentQueue(nullptr)
	, descriptorPool(nullptr)
	, current_frame(0)
	, window(window)
	, frameBufferSize(window.getFrameBufferSize())

	, vertexBuffer(nullptr)
	, vbDeviceMemory(nullptr)
	, indexBuffer(nullptr)
	, ibDeviceMemory(nullptr)
{}

v3d::vulkan::Context::~Context()
{
	release();
}

bool v3d::vulkan::Context::init(const v3d::glfw::Window& window, const bool enableValidationLayer)
{
	// Init logger
	auto& logger = v3d::Logger::getInstance();

	// Get version
	uint32_t major, minor, patch;
	if (!vulkan::utils::getVersion(major, minor, patch)) { logger.critical("Failed to get Context version."); return false; }
	logger.trace("Context version: " + std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch));

	validationLayerEnabled = enableValidationLayer;

	// temp
	auto& vertices = vertexData.getVertexData();
	vertices.push_back(v3d::vulkan::Vertex({ -0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }));
	vertices.push_back(v3d::vulkan::Vertex({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f }));
	vertices.push_back(v3d::vulkan::Vertex({ 0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }));
	vertices.push_back(v3d::vulkan::Vertex({ 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }));

	auto& indices = indexData.getVertexData();
	indices = std::vector<uint16_t>({ 0,1,2,3,2,1 });
	// temp

	if (!initInstance(window)) return false;
	if (validationLayerEnabled) if (!initDebugReport() || !initDebugUtilsMessenger()) return false;
	if (!initSurface(window)) return false;
	if (!initPhysicalDevice()) return false;
	if (!initDevice()) return false;
	if (!initSwapChain()) return false;
	if (!initSwapChainImages()) return false;
	if (!initRenderPass()) return false;
	if (!initGraphicsPipeline()) return false;
	if (!initFrameBuffer()) return false;
	if (!initCommandPool()) return false;

	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffer();

	if (!initCommandBuffer()) return false;
	if (!initSemaphore()) return false;
	if (!initFences()) return false;
	if (!initQueue()) return false;
	if (!initDescriptorPool()) return false;

	return true;
}

bool v3d::vulkan::Context::initInstance(const v3d::glfw::Window& window)
{
	instance = new (std::nothrow) v3d::vulkan::Instance();
	if (!instance) { v3d::Logger::getInstance().bad_alloc<Instance>(); return false; }
	if (!instance->init(window, validationLayerEnabled)) return false;
	return true;
}

bool v3d::vulkan::Context::initDebugReport()
{
	debugReportCallback = new (std::nothrow) v3d::vulkan::DebugReportCallback();
	if (debugReportCallback == nullptr) { v3d::Logger::getInstance().bad_alloc<DebugReportCallback>(); return false; }
	if (!debugReportCallback->init(*instance)) return false;
	return true;
}

bool v3d::vulkan::Context::initDebugUtilsMessenger()
{
	debugUtilsMessenger = new (std::nothrow) v3d::vulkan::DebugUtilsMessenger();
	if (debugUtilsMessenger == nullptr) { v3d::Logger::getInstance().bad_alloc<DebugUtilsMessenger>(); return false; }
	if (!debugUtilsMessenger->init(*instance)) return false;
	return true;
}

bool v3d::vulkan::Context::initSurface(const v3d::glfw::Window& window)
{
	VkSurfaceKHR cVkSurfaceKHR;
	if (!window.createWindowSurface(*instance, cVkSurfaceKHR)) return false;
	surface = vk::SurfaceKHR(cVkSurfaceKHR);
	return true;
}

bool v3d::vulkan::Context::initPhysicalDevice()
{
	physicalDevice = new (std::nothrow) v3d::vulkan::PhysicalDevice();
	if (!physicalDevice) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::PhysicalDevice>(); return false; }
	if (!physicalDevice->init(*instance, surface)) return false;

	return true;
}

bool v3d::vulkan::Context::initDevice()
{
	device = new (std::nothrow) v3d::vulkan::Device();
	if (!device) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Device>(); return false; }
	if (!device->init(*physicalDevice)) return false;
	return true;
}

bool v3d::vulkan::Context::initSwapChain()
{
	swapChain = new (std::nothrow) v3d::vulkan::SwapChain();
	if (swapChain == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::SwapChain>(); return false; }
	if (!swapChain->init(*physicalDevice, *device, surface, window)) return false;
	return true;
}

bool v3d::vulkan::Context::initSwapChainImages()
{
	images = device->getSwapchainImagesKHR(*swapChain);

	imageViews.reserve(images.size());
	vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
	vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
	for (auto image : images)
	{
		vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, swapChain->getFormat(), componentMapping, subResourceRange);
		imageViews.push_back(std::move(device->createImageView(imageViewCreateInfo)));
	}

	return true;
}

bool v3d::vulkan::Context::initRenderPass()
{
	vk::AttachmentDescription attachmentDescriptions
	(
		vk::AttachmentDescriptionFlags(),
		swapChain->getFormat(),
		vk::SampleCountFlagBits::e1,
		vk::AttachmentLoadOp::eClear,
		vk::AttachmentStoreOp::eStore,
		vk::AttachmentLoadOp::eDontCare,
		vk::AttachmentStoreOp::eDontCare,
		vk::ImageLayout::eUndefined,
		vk::ImageLayout::ePresentSrcKHR
	);

	vk::AttachmentReference colorAttachment(0, vk::ImageLayout::eColorAttachmentOptimal);

	// @note visit later
	//vk::AttachmentReference depthAttachment(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

	vk::SubpassDescription subpassDescription
	(
		vk::SubpassDescriptionFlags(),
		vk::PipelineBindPoint::eGraphics,
		0, nullptr,
		1, &colorAttachment//,
		//nullptr, 
		//(depthFormat != vk::Format::eUndefined) ? &depthAttachment : nullptr
	);

	vk::RenderPassCreateInfo createInfo
	(
		vk::RenderPassCreateFlags(),
		1, &attachmentDescriptions,
		1, &subpassDescription
	);

	renderPass = device->createRenderPass(createInfo);

	return true;
}

bool v3d::vulkan::Context::initGraphicsPipeline()
{
	pipeline = new (std::nothrow) v3d::vulkan::Pipeline();
	if (pipeline == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Pipeline>(); return false; }
	if (!pipeline->init(*device, *swapChain, renderPass)) return false;
	return true;
}

bool v3d::vulkan::Context::initFrameBuffer()
{
	const std::size_t size = imageViews.size();
	const auto& extent = swapChain->getExtent2D();

	framebuffers.resize(size);

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

		framebuffers[i] = device->createFrameBuffer(createInfo);
	}

	return true;
}

bool v3d::vulkan::Context::initCommandPool()
{
	const uint32_t graphicsFamilyIndex = physicalDevice->getGraphicsQueueFamilyIndex();

	vk::CommandPoolCreateInfo createInfo
	(
		vk::CommandPoolCreateFlags(),
		graphicsFamilyIndex
	);

	commandPool = device->createCommandPool(createInfo);

	return true;
}

bool v3d::vulkan::Context::initSemaphore()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		imageAvailableSemaphores.push_back(device->createSemaphore(vk::SemaphoreCreateInfo(vk::SemaphoreCreateFlags())));
		renderFinishedSemaphores.push_back(device->createSemaphore(vk::SemaphoreCreateInfo(vk::SemaphoreCreateFlags())));
	}

	return true;
}

bool v3d::vulkan::Context::initFences()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		frameFences.push_back(device->createFence(vk::FenceCreateInfo(vk::FenceCreateFlags(vk::FenceCreateFlagBits::eSignaled))));
	}

	return true;
}

bool v3d::vulkan::Context::initQueue()
{
	graphicsQueue = new (std::nothrow) v3d::vulkan::Queue();
	if (graphicsQueue == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Queue>(); return false; }
	if (!graphicsQueue->init(*device, physicalDevice->getGraphicsQueueFamilyIndex())) return false;

	presentQueue = new (std::nothrow) v3d::vulkan::Queue();
	if (presentQueue == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Queue>(); return false; }
	if (!presentQueue->init(*device, physicalDevice->getPresentQueueFamilyIndex())) return false;
	return true;
}

bool v3d::vulkan::Context::initCommandBuffer()
{
	const std::size_t fbSize = framebuffers.size();

	vk::CommandBufferAllocateInfo allocInfo
	(
		commandPool,
		vk::CommandBufferLevel::ePrimary,
		static_cast<uint32_t>(fbSize)
	);

	const std::vector<vk::CommandBuffer> cbs = device->allocateCommandBuffers(allocInfo);

	for (std::size_t i = 0; i < fbSize; i++)
	{
		auto newCB = new v3d::vulkan::CommandBuffer(cbs[i]);
		commandBuffers.push_back(newCB);
		newCB->record(framebuffers[i], renderPass, *swapChain, *pipeline, vertexBuffer, indexBuffer, static_cast<uint32_t>(indexData.getSize()));
		//newCB->record(framebuffers[i]->get(), renderPass->get(), *swapChain, *pipeline, *vertexBuffer, static_cast<uint32_t>(vertexData.getSize()));
	}

	return true;
}

bool v3d::vulkan::Context::initDescriptorPool()
{
	vk::DescriptorPoolSize poolSize
	(
		vk::DescriptorType::eUniformBuffer,
		static_cast<uint32_t>(images.size())
	);

	vk::DescriptorPoolCreateInfo poolInfo
	(
		vk::DescriptorPoolCreateFlags(),
		static_cast<uint32_t>(images.size()),
		1,
		&poolSize
	);

	descriptorPool = device->createDescriptorPool(poolInfo);

	return true;
}

bool v3d::vulkan::Context::recreateSwapChain()
{
	device->waitIdle();

	releaseSwapChain();

	if (!initSwapChain()) return false;
	if (!initSwapChainImages()) return false;
	if (!initRenderPass()) return false;
	if (!initGraphicsPipeline()) return false;
	if (!initFrameBuffer()) return false;
	if (!initCommandBuffer()) return false;
	if (!initDescriptorPool()) return false;
	createUniformBuffer();

	v3d::Logger::getInstance().info("Recreated swapchain");

	return true;
}

vk::Buffer v3d::vulkan::Context::createBuffer(const uint64_t size, const vk::BufferUsageFlags usageFlags) const
{
	vk::BufferCreateInfo createInfo
	(
		vk::BufferCreateFlags(),
		size,
		usageFlags
	);

	return device->createBuffer(createInfo);
}

vk::DeviceMemory v3d::vulkan::Context::createDeviceMemory(const vk::Buffer& buffer, const vk::MemoryPropertyFlags memoryPropertyFlags) const
{
	const vk::MemoryRequirements memRequirment = device->getMemoryRequirement(buffer);
	const vk::MemoryAllocateInfo allocInfo
	(
		memRequirment.size,
		physicalDevice->getMemoryTypeIndex(memRequirment.memoryTypeBits, memoryPropertyFlags)
	);

	vk::DeviceMemory deviceMemory =  device->allocateBuffer(allocInfo);
	device->bindBufferMemory(buffer, deviceMemory);
	return deviceMemory;
}

void v3d::vulkan::Context::copyBuffer(const vk::Buffer& src, const vk::Buffer& dst, const vk::DeviceSize size)
{
	vk::CommandBufferAllocateInfo allocInfo
	(
		commandPool,
		vk::CommandBufferLevel::ePrimary,
		1
	);
	vk::CommandBuffer cb = device->allocateCommandBuffer(allocInfo);

	vk::CommandBufferBeginInfo beginInfo
	(
		vk::CommandBufferUsageFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit),
		nullptr
	);

	cb.begin(beginInfo);

	vk::BufferCopy copyRegion(0, 0, size);

	cb.copyBuffer(src, dst, 1, &copyRegion);
	cb.end();

	vk::SubmitInfo submitInfo
	(
		0,
		nullptr,
		nullptr,
		1,
		&cb
	);

	graphicsQueue->submit(submitInfo);
	graphicsQueue->waitIdle();

	device->freeCommandBuffer(commandPool, cb);
}

void v3d::vulkan::Context::createVertexBuffer()
{
	vertexBuffer = createBuffer(vertexData.getDataSize(), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer);
	vbDeviceMemory = createDeviceMemory(vertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal);

	vk::Buffer stagingBuffer = createBuffer(vertexData.getDataSize(), vk::BufferUsageFlagBits::eTransferSrc);
	vk::DeviceMemory stagingDeviceMemory = createDeviceMemory(stagingBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

	void* data = device->mapMemory(stagingDeviceMemory, vertexData.getDataSize());
	memcpy(data, vertexData.getData(), vertexData.getDataSize());
	device->unMapMemory(stagingDeviceMemory);

	copyBuffer(stagingBuffer, vertexBuffer, vertexData.getDataSize());

	device->get().destroyBuffer(stagingBuffer);
	device->get().freeMemory(stagingDeviceMemory);
}

void v3d::vulkan::Context::createIndexBuffer()
{
	indexBuffer = createBuffer(indexData.getDataSize(), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer);
	ibDeviceMemory = createDeviceMemory(indexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal);

	vk::Buffer stagingBuffer = createBuffer(indexData.getDataSize(), vk::BufferUsageFlagBits::eTransferSrc);
	vk::DeviceMemory stagingDeviceMemory = createDeviceMemory(stagingBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

	void* data = device->mapMemory(stagingDeviceMemory, indexData.getDataSize());
	memcpy(data, indexData.getData(), indexData.getDataSize());
	device->unMapMemory(stagingDeviceMemory);

	copyBuffer(stagingBuffer, indexBuffer, indexData.getDataSize());

	device->get().destroyBuffer(stagingBuffer);
	device->get().freeMemory(stagingDeviceMemory);
}

void v3d::vulkan::Context::createUniformBuffer()
{
	const vk::DeviceSize bufferSize = sizeof(glm::mat4) * 3;
	const std::size_t size = imageViews.size();

	uniformBuffers.resize(size);
	ubDeviceMemories.resize(size);

	for (std::size_t i = 0; i < size; i++)
	{
		uniformBuffers.at(i) = createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer);
		ubDeviceMemories.at(i) = createDeviceMemory(uniformBuffers.at(i), vk::MemoryPropertyFlagBits::eHostCoherent);
	}


}

void v3d::vulkan::Context::updateUniformBuffer(const uint32_t imageIndex)
{
	static struct UniformBufferObject
	{
		glm::mat4 m, v, p;
	} ubo;
	ubo.m = ubo.v = ubo.p = glm::mat4(1);
	//ubo.m = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ubo.v = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//ubo.p = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
	//ubo.proj[1][1] *= -1;

	void* data = device->mapMemory(ubDeviceMemories[imageIndex], sizeof(ubo));
	memcpy(data, &ubo, sizeof(ubo));
	device->unMapMemory(ubDeviceMemories[imageIndex]);
}

void v3d::vulkan::Context::render()
{
	device->waitForFences(frameFences[current_frame]);

	const vk::ResultValue<uint32_t> result = device->acquireNextImage(*swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[current_frame]);
	if (result.result == vk::Result::eErrorOutOfDateKHR)
	{
		recreateSwapChain();
		return;
	}
	else if (result.result != vk::Result::eSuccess && result.result != vk::Result::eSuboptimalKHR)
	{ 
		throw std::runtime_error("failed to acquire swap chain image!"); 
	}
	assert(result.value < framebuffers.size());

	updateUniformBuffer(result.value);

	vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[current_frame] };
	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[current_frame] };
	vk::PipelineStageFlags waitFlags[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::CommandBuffer commandBuffers[] = { this->commandBuffers.at(result.value)->getHandle() };
	vk::SubmitInfo submitInfo
	(
		1,
		waitSemaphores,
		waitFlags,
		1,
		commandBuffers,
		1,
		signalSemaphores
	);

	device->resetFences(frameFences[current_frame]);

	graphicsQueue->submit(submitInfo, frameFences[current_frame]);

	vk::SwapchainKHR swapChains[] = { swapChain->get() };
	vk::PresentInfoKHR presentInfo
	(
		1,
		signalSemaphores,
		1,
		swapChains,
		&result.value
	);

	const vk::Result presentResult = presentQueue->present(presentInfo);

	if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || frameBufferSize != window.getFrameBufferSize())
	{
		frameBufferSize = window.getFrameBufferSize();
		recreateSwapChain();
	}
	else if (presentResult != vk::Result::eSuccess)
	{
		throw std::runtime_error("Failed to present swap chain image");
	}

	current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void v3d::vulkan::Context::waitIdle()
{
	device->waitIdle();
}

const v3d::vulkan::Instance& v3d::vulkan::Context::getInstance() const
{
	return *instance;
}

void v3d::vulkan::Context::release()
{
	auto& logger = v3d::Logger::getInstance();
	logger.info("Releasing Context...");
	device->get().destroyBuffer(vertexBuffer);
	device->get().freeMemory(vbDeviceMemory);
	device->get().destroyBuffer(indexBuffer);
	device->get().freeMemory(ibDeviceMemory);
	for (auto& f : frameFences) { device->get().destroyFence(f); }
	frameFences.clear();
	SAFE_DELETE(graphicsQueue);
	SAFE_DELETE(presentQueue);
	for (auto& s : imageAvailableSemaphores) { device->get().destroySemaphore(s); }
	imageAvailableSemaphores.clear();
	for (auto& s : renderFinishedSemaphores) { device->get().destroySemaphore(s); }
	renderFinishedSemaphores.clear();
	releaseSwapChain();
	SAFE_DELETE(device);
	SAFE_DELETE(physicalDevice);
	instance->get().destroySurfaceKHR(surface);
	SAFE_DELETE(debugUtilsMessenger);
	SAFE_DELETE(debugReportCallback);
	SAFE_DELETE(instance);
	logger.info("Releasing Context finished");
}

void v3d::vulkan::Context::releaseSwapChain()
{
	for (std::size_t i = 0; i < images.size(); i++)
	{
		device->get().destroyBuffer(uniformBuffers.at(i));
		device->get().freeMemory(ubDeviceMemories.at(i));
	}
	uniformBuffers.clear();
	ubDeviceMemories.clear();
	device->get().destroyDescriptorPool(descriptorPool);
	for (auto& cb : commandBuffers) { device->freeCommandBuffer(commandPool, cb->getHandle()); SAFE_DELETE(cb); }
	commandBuffers.clear();
	device->get().destroyCommandPool(commandPool);
	for (auto& f : framebuffers) { device->get().destroyFramebuffer(f); }
	framebuffers.clear();
	SAFE_DELETE(pipeline);
	device->get().destroyRenderPass(renderPass);
	for (auto& imageView : imageViews) { device->get().destroyImageView(imageView); }
	imageViews.clear();
	images.clear();
	SAFE_DELETE(swapChain);
}
