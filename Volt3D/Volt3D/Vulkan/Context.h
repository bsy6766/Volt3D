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
#include <memory>

#include "utils/Macros.h"
#include "Config/BuildConfig.h"

#include "Renderer/Vertex.h"
#include "Renderer/VertexData.h"

#include "Spritesheet/Image.h"

namespace v3d
{
	namespace glfw { class Window; }
	namespace vulkan
	{
		class View;
		class Instance;
		class DebugReportCallback;
		class DebugUtilsMessenger;
		class Devices;
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
			Context( const v3d::glfw::Window& window );

			// Context instance
			v3d::vulkan::Instance* instance;

			bool validationLayerEnabled;
			v3d::vulkan::DebugReportCallback* debugReportCallback;
			v3d::vulkan::DebugUtilsMessenger* debugUtilsMessenger;

			vk::SurfaceKHR surface;

			std::shared_ptr<v3d::vulkan::Devices> devices;
			vk::PhysicalDevice physicalDevice;
			vk::Device logicalDevice;

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
			vk::Queue graphicsQueue;
			vk::Queue presentQueue;
			std::vector<v3d::vulkan::CommandBuffer*> commandBuffers;

			vk::DescriptorSetLayout descriptorLayout;
			vk::DescriptorPool descriptorPool;
			std::vector<vk::DescriptorSet> descriptorSets;

			const int MAX_FRAMES_IN_FLIGHT = 2;
			int current_frame;
			glm::uvec2 frameBufferSize;

			const v3d::glfw::Window& window;

			// vertex & index buffer
			struct BufferData
			{
			public:
				vk::Buffer vertexBuffer;
				vk::Buffer indexBuffer;
				vk::DeviceMemory vbDeviceMemory;
				vk::DeviceMemory ibDeviceMemory;
				v3d::VertexData<v3d::V3_C4_T2> vertexData;
				v3d::VertexData<uint16_t> indexData;
			};

			BufferData lenaBuffer;

			struct UniformBufferObject
			{
				std::vector<vk::Buffer> buffers;
				std::vector<vk::DeviceMemory> deviceMemories;
			} typedef UBO;

			UBO mvpUBO;
			UBO dissolveUBO;

			struct Texture
			{
			public:
				vk::Image image;
				vk::ImageView imageView;
				vk::Sampler sampler;
				vk::DeviceMemory deviceMemory;
				v3d::Image* imageSource;
			};

			Texture lena;
			Texture RGBW;

			bool initInstance( const v3d::glfw::Window& window );
			bool initDebugReport();
			bool initDebugUtilsMessenger();
			bool initSurface( const v3d::glfw::Window& window );
			bool initDevices();
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
			bool initDescriptorLayout();
			bool initDescriptorPool();
			bool initDescriptorSet();
			bool recreateSwapChain();
			void release();
			void releaseSwapChain();

			v3d::vulkan::CommandBuffer createCommandBuffer( const vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary );

			void copyBuffer( const vk::Buffer& src, const vk::Buffer& dst, const vk::DeviceSize size );
			void createVertexBuffer( vk::Buffer& vBuffer, vk::DeviceMemory& vbDeviceMemory, const uint32_t vbDataSize, const void * vbData );
			void createIndexBuffer( vk::Buffer& iBuffer, vk::DeviceMemory& ibDeviceMemory, const uint32_t ibDataSize, const void* ibData );

			void createUniformBuffer( UBO& ubo, const std::size_t uboDataSize );
			void updateMVPUBO( const uint32_t imageIndex );
			void updateDissolveUBO( const uint32_t imageIndex );

			void createTexture( const char* path, v3d::vulkan::Context::Texture& texture );
			void createTextureImage( const char* path, v3d::vulkan::Context::Texture& texture );
			void createTextureImageView( v3d::vulkan::Context::Texture& texture );
			void createImage( const std::size_t w, const std::size_t h, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags, const vk::MemoryPropertyFlags memoryPropertyFlags, vk::Image& image, vk::DeviceMemory& deviceMemory );
			void transitionImageLayout( vk::Image& image, const vk::Format& format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout );
			void copyBufferToImage( vk::Buffer& buffer, vk::Image& dst, const uint32_t width, const uint32_t height );
			vk::ImageView createImageView( vk::Image& image, const vk::Format& format );
			void oneTimeSubmit( v3d::vulkan::CommandBuffer& cb );

			void render();
			void waitIdle();

		public:
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Context );
			DEFAULT_MOVE_CONSTRUCTORS( Context );
			~Context();

			bool init( const v3d::glfw::Window& window, const bool enableValidationLayer );

			const v3d::vulkan::Instance& getInstance() const;
			std::shared_ptr<v3d::vulkan::Devices> getDevices() const;
		};
	}
}

#endif