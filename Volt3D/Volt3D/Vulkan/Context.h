/**
*	@file Context.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_CONTEXT_H
#define V3D_VK_CONTEXT_H

#include <vulkan/vulkan.hpp>

#include <vector>

#include "utils/Macros.h"
#include "Config/BuildConfig.h"

#include "Vertex.h"
#include "Renderer/VertexData.h"

namespace v3d
{
	namespace glfw { class Window; }
	namespace vulkan
	{
		class View;
		class Instance;
		class DebugReportCallback;
		class DebugUtilsMessenger;
		class PhysicalDevice;
		class Device;
		class SwapChain;
		class RenderPass;
		class Pipeline;
		class Semaphore;
		class Fence;
		class Queue;
		class CommandBuffer;

		class Buffer;
		class DeviceMemory;

		/**
		*	@class Context
		*	@brief Vulkan context.
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Context
		{
			friend class Engine;

		private:
			Context(const v3d::glfw::Window& window);

			// Context instance
			v3d::vulkan::Instance* instance;

			bool validationLayerEnabled;
			v3d::vulkan::DebugReportCallback* debugReportCallback;
			v3d::vulkan::DebugUtilsMessenger* debugUtilsMessenger;
			vk::SurfaceKHR surface;
			v3d::vulkan::PhysicalDevice* physicalDevice;
			v3d::vulkan::Device* device;
			v3d::vulkan::SwapChain* swapChain;
			v3d::vulkan::RenderPass* renderPass;
			v3d::vulkan::Pipeline* pipeline;
			std::vector<vk::Framebuffer> framebuffers;
			vk::CommandPool commandPool;
			std::vector<v3d::vulkan::Semaphore*> imageAvailableSemaphores;
			std::vector<v3d::vulkan::Semaphore*> renderFinishedSemaphores;
			std::vector<v3d::vulkan::Fence*> frameFences;
			v3d::vulkan::Queue* graphicsQueue;
			v3d::vulkan::Queue* presentQueue;
			std::vector<v3d::vulkan::CommandBuffer*> commandBuffers;

			const int MAX_FRAMES_IN_FLIGHT = 2;
			int current_frame;
			glm::uvec2 frameBufferSize;

			const v3d::glfw::Window& window;

			v3d::vulkan::Buffer* vertexBuffer;
			v3d::vulkan::Buffer* indexBuffer;
			v3d::vulkan::DeviceMemory* vbDeviceMemory;
			v3d::vulkan::DeviceMemory* ibDeviceMemory;
			v3d::VertexData<v3d::vulkan::Vertex> vertexData;
			v3d::VertexData<uint16_t> indexData;

			bool initInstance(const v3d::glfw::Window& window);
			bool initDebugReport();
			bool initDebugUtilsMessenger();
			bool initSurface(const v3d::glfw::Window& window);
			bool initPhysicalDevice();
			bool initDevice();
			bool initSwapChain();
			bool initRenderPass();
			bool initGraphicsPipeline();
			bool initFrameBuffer();
			bool initCommandPool();
			bool initSemaphore();
			bool initFences();
			bool initQueue();
			bool initCommandBuffer();
			bool recreateSwapChain();
			void release();
			
			v3d::vulkan::Buffer* createBuffer(const vk::BufferUsageFlags usageFlags);
			v3d::vulkan::DeviceMemory* createDeviceMemory(const v3d::vulkan::Buffer& buffer, const vk::MemoryPropertyFlags memoryPropertyFlags) const;
			void copyBuffer(const v3d::vulkan::Buffer& src, const v3d::vulkan::Buffer& dst, const vk::DeviceSize size);

			void render();
			void waitIdle();

		public:
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Context);
			DEFAULT_MOVE_CONSTRUCTORS(Context);
			~Context();

			bool init(const v3d::glfw::Window& window, const bool enableValidationLayer);

			const v3d::vulkan::Instance& getInstance() const;
			const vk::SurfaceKHR& getSurface() const;
			const v3d::vulkan::PhysicalDevice& getPhysicalDevice() const;
			const v3d::vulkan::Device& getDevice() const;
			const v3d::vulkan::SwapChain& getSwapChain() const;
			const v3d::vulkan::RenderPass& getRenderPass() const;
			const v3d::vulkan::Pipeline& getPipeline() const;
			const vk::CommandPool& getCommandPool() const;
		};
	}
}

#endif