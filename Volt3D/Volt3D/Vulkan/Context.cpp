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
#include "Shader.h"
#include "RenderPass.h"
#include "Pipeline.h"
#include "FrameBuffer.h"
#include "CommandPool.h"
#include "Semaphore.h"
#include "Queue.h"
#include "Utils.h"
#include "Config/BuildConfig.h"

v3d::vulkan::Context::Context()
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
	, imageAvailableSemaphore(nullptr)
	, renderFinishedSemaphore(nullptr)
	, graphicsQueue(nullptr)
	, presentQueue(nullptr)
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

	if (!initInstance(window)) return false;
	if (validationLayerEnabled) if (!initDebugReport() || !initDebugUtilsMessenger()) return false;
	if (!initSurface(window)) return false;
	if (!initPhysicalDevice()) return false;
	if (!initDevice()) return false;
	if (!initSwapChain()) return false;
	if (!initRenderPass()) return false;
	if (!initGraphicsPipeline()) return false;
	if (!initFrameBuffer()) return false;
	if (!initCommandPool()) return false;
	if (!initSemaphore()) return false;
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
	if (!swapChain->init(*physicalDevice, *device, *surface)) return false;
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
	if (!commandPool->init(*physicalDevice, *device, *frameBuffer, *renderPass, *swapChain, *pipeline)) return false;
	return true;
}

bool v3d::vulkan::Context::initSemaphore()
{
	imageAvailableSemaphore = new (std::nothrow) v3d::vulkan::Semaphore();
	if (imageAvailableSemaphore == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Semaphore>(); return false; }
	if (!imageAvailableSemaphore->init(*device)) return false;

	renderFinishedSemaphore = new (std::nothrow) v3d::vulkan::Semaphore();
	if (renderFinishedSemaphore == nullptr) { v3d::Logger::getInstance().bad_alloc<v3d::vulkan::Semaphore>(); return false; }
	if (!renderFinishedSemaphore->init(*device)) return false;

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

void v3d::vulkan::Context::render()
{
	const uint32_t imageIndex = device->acquireNextImage(*swapChain, std::numeric_limits<uint64_t>::max(), *imageAvailableSemaphore);
	assert(imageIndex < frameBuffer->size());

	vk::Semaphore waitSemaphores[] = { imageAvailableSemaphore->get() };
	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphore->get() };
	vk::PipelineStageFlags waitFlags[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::CommandBuffer commandBuffers[] = { commandPool->getBufferAt(imageIndex).get() };
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

	graphicsQueue->submit(submitInfo);

	vk::SwapchainKHR swapChains[] = { swapChain->get() };
	vk::PresentInfoKHR presentInfo
	(
		1,
		signalSemaphores,
		1,
		swapChains,
		&imageIndex
	);

	presentQueue->present(presentInfo);
	presentQueue->waitIdle();
}

void v3d::vulkan::Context::waitIdle()
{
	device->get().waitIdle();
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
	SAFE_DELETE(graphicsQueue);
	SAFE_DELETE(presentQueue);
	SAFE_DELETE(imageAvailableSemaphore);
	SAFE_DELETE(renderFinishedSemaphore);
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
