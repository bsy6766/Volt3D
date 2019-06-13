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
#include "Surface.h"
#include "PhysicalDevice.h"
#include "Device.h"
#include "SwapChain.h"
#include "ShaderModule.h"
#include "RenderPass.h"
#include "Pipeline.h"
#include "FrameBuffer.h"
#include "CommandPool.h"
#include "Semaphore.h"
#include "Fence.h"
#include "Queue.h"
#include "Utils.h"
#include "Config/BuildConfig.h"

#include "Buffer.h"
#include "DeviceMemory.h"
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
	, frameBuffer(nullptr)
	, commandPool(nullptr)
	, imageAvailableSemaphores()
	, renderFinishedSemaphores()
	, frameFences()
	, graphicsQueue(nullptr)
	, presentQueue(nullptr)
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
	if (!initRenderPass()) return false;
	if (!initGraphicsPipeline()) return false;
	if (!initFrameBuffer()) return false;

	{
		vertexBuffer = createBuffer(vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer);
		vbDeviceMemory = createDeviceMemory(*vertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal);

		v3d::vulkan::Buffer* vertexStagingBuffer = createBuffer(vk::BufferUsageFlagBits::eTransferSrc);
		v3d::vulkan::DeviceMemory* vsbDeviceMemory = createDeviceMemory(*vertexStagingBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

		void* data = device->mapMemory(*vsbDeviceMemory, vertexData.getDataSize());
		memcpy(data, vertexData.getData(), vertexData.getDataSize());
		device->unMapMemory(*vsbDeviceMemory);

		copyBuffer(*vertexStagingBuffer, *vertexBuffer, vertexData.getDataSize());

		delete vertexStagingBuffer;
		delete vsbDeviceMemory;
	}

	{
		indexBuffer = createBuffer(vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer);
		ibDeviceMemory = createDeviceMemory(*indexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal);

		v3d::vulkan::Buffer* indexStagingBuffer = createBuffer(vk::BufferUsageFlagBits::eTransferSrc);
		v3d::vulkan::DeviceMemory* isbDeviceMemory = createDeviceMemory(*indexStagingBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

		void* data = device->mapMemory(*isbDeviceMemory, indexData.getDataSize());
		memcpy(data, indexData.getData(), indexData.getDataSize());
		device->unMapMemory(*isbDeviceMemory);

		copyBuffer(*indexStagingBuffer, *vertexBuffer, indexData.getDataSize());

		delete indexStagingBuffer;
		delete isbDeviceMemory;
	}

	if (!initCommandPool()) return false;
	if (!initSemaphore()) return false;
	if (!initFences()) return false;
	if (!initQueue()) return false;

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
	surface = new(std::nothrow) v3d::vulkan::Surface();
	if (!surface) v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Surface>();
	if (!surface->init(window, *instance)) return false;
	return true;
}

bool v3d::vulkan::Context::initPhysicalDevice()
{
	physicalDevice = new (std::nothrow) v3d::vulkan::PhysicalDevice();
	if (!physicalDevice) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::PhysicalDevice>(); return false; }
	if (!physicalDevice->init(*instance, *surface)) return false;

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
	if (!swapChain->init(*physicalDevice, *device, *surface, window)) return false;
	return true;
}

bool v3d::vulkan::Context::initRenderPass()
{
	renderPass = new (std::nothrow) v3d::vulkan::RenderPass();
	if (renderPass == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::RenderPass>(); return false; }
	if (!renderPass->init(*device, *swapChain)) return false;
	return true;
}

bool v3d::vulkan::Context::initGraphicsPipeline()
{
	pipeline = new (std::nothrow) v3d::vulkan::Pipeline();
	if (pipeline == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Pipeline>(); return false; }
	if (!pipeline->init(*device, *swapChain, *renderPass)) return false;
	return true;
}

bool v3d::vulkan::Context::initFrameBuffer()
{
	frameBuffer = new (std::nothrow) v3d::vulkan::FrameBuffer();
	if (frameBuffer == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::FrameBuffer>(); return false; }
	if (!frameBuffer->init(*swapChain, *renderPass, *device)) return false;
	return true;
}

bool v3d::vulkan::Context::initCommandPool()
{
	commandPool = new (std::nothrow) v3d::vulkan::CommandPool();
	if (commandPool == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::CommandPool>(); return false; }
	if (!commandPool->init(*physicalDevice, *device)) return false;
	if (!commandPool->initCommandBuffers(*device, *frameBuffer)) return false;

	//commandPool->record(*frameBuffer, *renderPass, *swapChain, *pipeline, *vertexBuffer, static_cast<uint32_t>(vertexData.getSize()));
	commandPool->record(*frameBuffer, *renderPass, *swapChain, *pipeline, *vertexBuffer, *indexBuffer, static_cast<uint32_t>(indexData.getSize()));
	
	return true;
}

bool v3d::vulkan::Context::initSemaphore()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		{
			v3d::vulkan::Semaphore* semaphore = new (std::nothrow) v3d::vulkan::Semaphore();
			if (semaphore == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Semaphore>(); return false; }
			if (!semaphore->init(*device)) return false;
			imageAvailableSemaphores.push_back(semaphore);
		}

		{
			v3d::vulkan::Semaphore* semaphore = new (std::nothrow) v3d::vulkan::Semaphore();
			if (semaphore == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Semaphore>(); return false; }
			if (!semaphore->init(*device)) return false;
			renderFinishedSemaphores.push_back(semaphore);
		}
	}

	return true;
}

bool v3d::vulkan::Context::initFences()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		v3d::vulkan::Fence* fence = new (std::nothrow) v3d::vulkan::Fence();
		if (fence == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Fence>(); return false; }
		if (!fence->init(*device)) return false;
		frameFences.push_back(fence);
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

bool v3d::vulkan::Context::recreateSwapChain()
{
	device->waitIdle();

	SAFE_DELETE(frameBuffer);
	SAFE_DELETE(pipeline);
	SAFE_DELETE(renderPass);
	SAFE_DELETE(swapChain);
	device->freeCommandBuffers(*commandPool);
	//commandPool->clearCommandBuffers();

	if (!initSwapChain()) return false;
	if (!initRenderPass()) return false;
	if (!initGraphicsPipeline()) return false;
	if (!initFrameBuffer()) return false;
	if (!commandPool->initCommandBuffers(*device, *frameBuffer)) return false;
	commandPool->record(*frameBuffer, *renderPass, *swapChain, *pipeline, *vertexBuffer, static_cast<uint32_t>(vertexData.getSize()));

	v3d::Logger::getInstance().info("Recreated swapchain");

	return true;
}

v3d::vulkan::Buffer* v3d::vulkan::Context::createBuffer(const vk::BufferUsageFlags usageFlags)
{
	auto newBuffer = NO_THROW_NEW(Buffer);
	newBuffer->init(*device, vertexData.getDataSize(), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer);
	return newBuffer;
}

v3d::vulkan::DeviceMemory* v3d::vulkan::Context::createDeviceMemory(const v3d::vulkan::Buffer& buffer, const vk::MemoryPropertyFlags memoryPropertyFlags) const
{
	auto newDeviceMemory = NO_THROW_NEW(DeviceMemory);
	newDeviceMemory->init(*device, *physicalDevice, buffer, memoryPropertyFlags);
	device->bindBufferMemory(buffer, *newDeviceMemory);
	return newDeviceMemory;
}

void v3d::vulkan::Context::copyBuffer(const v3d::vulkan::Buffer& src, const v3d::vulkan::Buffer& dst, const vk::DeviceSize size)
{
	vk::CommandBufferAllocateInfo allocInfo
	(
		commandPool->get(),
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

	vk::BufferCopy copyRegion(0, 0, vertexData.getDataSize());

	cb.copyBuffer(src.get(), dst.get(), 1, &copyRegion);
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

	device->freeCommandBuffer(*commandPool, cb);
}

void v3d::vulkan::Context::render()
{
	device->waitForFences(*frameFences[current_frame]);

	const vk::ResultValue<uint32_t> result = device->acquireNextImage(*swapChain, std::numeric_limits<uint64_t>::max(), *imageAvailableSemaphores[current_frame]);
	if (result.result == vk::Result::eErrorOutOfDateKHR)
	{
		recreateSwapChain();
		return;
	}
	else if (result.result != vk::Result::eSuccess && result.result != vk::Result::eSuboptimalKHR)
	{ 
		throw std::runtime_error("failed to acquire swap chain image!"); 
	}
	assert(result.value < frameBuffer->size());

	vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[current_frame]->get() };
	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[current_frame]->get() };
	vk::PipelineStageFlags waitFlags[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::CommandBuffer commandBuffers[] = { commandPool->getBufferAt(result.value) };
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

	device->resetFences(*frameFences[current_frame]);

	graphicsQueue->submit(submitInfo, *frameFences[current_frame]);

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

const v3d::vulkan::Surface& v3d::vulkan::Context::getSurface() const
{
	return *surface;
}

const v3d::vulkan::PhysicalDevice& v3d::vulkan::Context::getPhysicalDevice() const
{
	return *physicalDevice;
}

const v3d::vulkan::Device& v3d::vulkan::Context::getDevice() const
{
	return *device;
}

const v3d::vulkan::SwapChain& v3d::vulkan::Context::getSwapChain() const
{
	return *swapChain;
}

const v3d::vulkan::RenderPass& v3d::vulkan::Context::getRenderPass() const
{
	return *renderPass;
}

const v3d::vulkan::Pipeline& v3d::vulkan::Context::getPipeline() const
{
	return *pipeline;
}

const v3d::vulkan::FrameBuffer& v3d::vulkan::Context::getFrameBuffer() const
{
	return *frameBuffer;
}

const v3d::vulkan::CommandPool& v3d::vulkan::Context::getCommandPool() const
{
	return *commandPool;
}

void v3d::vulkan::Context::release()
{
	auto& logger = v3d::Logger::getInstance();
	logger.info("Releasing Context...");
	SAFE_DELETE(vbDeviceMemory);
	SAFE_DELETE(vertexBuffer);
	SAFE_DELETE(ibDeviceMemory);
	SAFE_DELETE(indexBuffer);
	for (auto& f : frameFences) { SAFE_DELETE(f); }
	SAFE_DELETE(graphicsQueue);
	SAFE_DELETE(presentQueue);
	for (auto& s : imageAvailableSemaphores) { SAFE_DELETE(s); }
	imageAvailableSemaphores.clear();
	for (auto& s : renderFinishedSemaphores) { SAFE_DELETE(s); }
	renderFinishedSemaphores.clear();
	SAFE_DELETE(commandPool);
	SAFE_DELETE(frameBuffer);
	SAFE_DELETE(pipeline);
	SAFE_DELETE(renderPass);
	SAFE_DELETE(swapChain);
	SAFE_DELETE(device);
	SAFE_DELETE(physicalDevice);
	SAFE_DELETE(surface);
	SAFE_DELETE(debugUtilsMessenger);
	SAFE_DELETE(debugReportCallback);
	SAFE_DELETE(instance);
	logger.info("Releasing Context finished");
}