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

V3D_NS_BEGIN

class Image;
namespace glfw { class Window; }

VK_NS_BEGIN

class View;
class Instance;
class PhysicalDevice;
class LogicalDevice;
class SwapChain;
class Pipeline;
class Queue;
class CommandBuffer;
class CommandPool;
class Buffer;
class UniformBuffer;

/**
*	@class Context
*	@brief Vulkan context.
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL Context
{
	friend class Engine;

private:
	Context();

	// Context instance
	v3d::vulkan::Instance* instance;

	vk::SurfaceKHR surface;

	v3d::vulkan::PhysicalDevice* physicalDevice;
	v3d::vulkan::LogicalDevice* logicalDevice;

	v3d::vulkan::SwapChain* swapChain;
	std::vector<vk::Image> images;
	std::vector<vk::ImageView> imageViews;

	vk::RenderPass renderPass;
	v3d::vulkan::Pipeline* pipeline;

	std::vector<vk::Framebuffer> framebuffers;
	std::vector<vk::Semaphore> imageAvailableSemaphores;
	std::vector<vk::Semaphore> renderFinishedSemaphores;
	std::vector<vk::Fence> frameFences;

	v3d::vulkan::CommandPool* commandPool;
	std::vector<v3d::vulkan::CommandBuffer*> commandBuffers;

	vk::DescriptorSetLayout descriptorLayout;
	vk::DescriptorPool descriptorPool;
	std::vector<vk::DescriptorSet> descriptorSets;

	const int MAX_FRAMES_IN_FLIGHT = 2;
	int current_frame;
	glm::uvec2 frameBufferSize;

	v3d::glfw::Window* window;




	struct LenaBuffer
	{
		v3d::vulkan::Buffer* vertexBuffer;
		v3d::vulkan::Buffer* indexBuffer;
		v3d::VertexData<v3d::V3_C4_T2> vertexData;
		v3d::VertexData<uint16_t> indexData;
	}lenaBuffer;

	std::vector<v3d::vulkan::UniformBuffer*> mvpUBOs;

	struct MVP
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
	};

	std::vector<v3d::vulkan::Context::MVP> mvps;

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

	bool init( const bool enableValidationLayer );
	bool initInstance( const bool enableValidationLayer );
	bool initSurface();
	bool initPhysicalDevice();
	bool initLogicalDevice();
	bool initSwapChain();
	bool initSwapChainImages();
	bool initRenderPass();
	bool initGraphicsPipeline();
	bool initFrameBuffer();
	bool initCommandPool();
	bool initSemaphore();
	bool initFences();
	bool initCommandBuffer();
	bool initDescriptorLayout();
	bool initDescriptorPool();
	bool initDescriptorSet();
	bool recreateSwapChain();
	void release();
	void releaseSwapChain();

	//v3d::vulkan::CommandBuffer createCommandBuffer( const vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary );

	void copyBuffer( const vk::Buffer& src, const vk::Buffer& dst, const vk::DeviceSize size );
	void createLenaBuffer();

	void createMVPUBO();
	void updateMVPUBO( const uint32_t imageIndex );

	void createTexture( const char* path, v3d::vulkan::Context::Texture& texture );
	void createTextureImage( const char* path, v3d::vulkan::Context::Texture& texture );
	void createTextureImageView( v3d::vulkan::Context::Texture& texture );
	void createImage( const uint32_t w, const uint32_t h, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags, const vk::MemoryPropertyFlags memoryPropertyFlags, vk::Image& image, vk::DeviceMemory& deviceMemory );
	void transitionImageLayout( vk::Image& image, const vk::Format& format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout );
	void copyBufferToImage( const vk::Buffer& buffer, vk::Image& dst, const uint32_t width, const uint32_t height );
	vk::ImageView createImageView( vk::Image& image, const vk::Format& format );

	void oneTimeSubmit( v3d::vulkan::CommandBuffer& cb );

	void render();
	void waitIdle();

public:
	~Context();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Context );
	DEFAULT_MOVE_CONSTRUCTORS( Context );

	/** Get Vulkan context */
	static v3d::vulkan::Context* get();

	/** Get Instance */
	v3d::vulkan::Instance* getInstance() const;

	/** Get SurfaceKHR */
	const vk::SurfaceKHR& getSurface() const;

	/** Get PhysicalDevice */
	v3d::vulkan::PhysicalDevice* getPhysicalDevice() const;

	/** Get LogicalDevice */
	v3d::vulkan::LogicalDevice* getLogicalDevice() const;

	/** Get CommandPool */
	v3d::vulkan::CommandPool* getCommandPool() const;
};

VK_NS_END
V3D_NS_END

#endif