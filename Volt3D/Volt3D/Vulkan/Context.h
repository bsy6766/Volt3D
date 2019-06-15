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
		class Pipeline;
		class Queue;
		class CommandBuffer;
		
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
			std::vector<vk::Image> images;
			std::vector<vk::ImageView> imageViews;
			vk::RenderPass renderPass;
			v3d::vulkan::Pipeline* pipeline;
			std::vector<vk::Framebuffer> framebuffers;
			vk::CommandPool commandPool;
			std::vector<vk::Semaphore> imageAvailableSemaphores;
			std::vector<vk::Semaphore> renderFinishedSemaphores;
			std::vector<vk::Fence> frameFences;
			v3d::vulkan::Queue* graphicsQueue;
			v3d::vulkan::Queue* presentQueue;
			std::vector<v3d::vulkan::CommandBuffer*> commandBuffers;
			vk::DescriptorPool descriptorPool;

			const int MAX_FRAMES_IN_FLIGHT = 2;
			int current_frame;
			glm::uvec2 frameBufferSize;

			const v3d::glfw::Window& window;

			// vertex & index buffer
			vk::Buffer vertexBuffer;
			vk::Buffer indexBuffer;
			vk::DeviceMemory vbDeviceMemory;
			vk::DeviceMemory ibDeviceMemory;
			v3d::VertexData<v3d::vulkan::Vertex> vertexData;
			v3d::VertexData<uint16_t> indexData;

			std::vector<vk::Buffer> uniformBuffers;
			std::vector<vk::DeviceMemory> ubDeviceMemories;

			bool initInstance(const v3d::glfw::Window& window);
			bool initDebugReport();
			bool initDebugUtilsMessenger();
			bool initSurface(const v3d::glfw::Window& window);
			bool initPhysicalDevice();
			bool initDevice();
			bool initSwapChain();
			bool initSwapChainImages();
			bool initRenderPass();
			bool initGraphicsPipeline();
			bool initFrameBuffer();
			bool initCommandPool();
			bool initSemaphore();
			bool initFences();
			bool initQueue();
			bool initCommandBuffer();
			bool initDescriptorPool();
			bool recreateSwapChain();
			void release();
			void releaseSwapChain();
			
			vk::Buffer createBuffer(const uint64_t size, const vk::BufferUsageFlags usageFlags) const;
			vk::DeviceMemory createDeviceMemory(const vk::Buffer& buffer, const vk::MemoryPropertyFlags memoryPropertyFlags) const;
			void copyBuffer(const vk::Buffer& src, const vk::Buffer& dst, const vk::DeviceSize size);
			void createVertexBuffer();
			void createIndexBuffer();

			void createUniformBuffer();
			void updateUniformBuffer(const uint32_t imageIndex);

			void render();
			void waitIdle();

		public:
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Context);
			DEFAULT_MOVE_CONSTRUCTORS(Context);
			~Context();

			bool init(const v3d::glfw::Window& window, const bool enableValidationLayer);

			const v3d::vulkan::Instance& getInstance() const;
		};
	}
}

#endif