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
#include "SwapChain.h"
#include "ShaderModule.h"
#include "Pipeline.h"
#include "Queue.h"
#include "CommandBuffer.h"
#include "Utils.h"
#include "Config/BuildConfig.h"

#include "Renderer/VertexData.cpp"

#include "Spritesheet/Image.h"

v3d::vulkan::Context::Context( const v3d::glfw::Window& window )
	: instance( nullptr )
	, validationLayerEnabled( false )
	, debugReportCallback( nullptr )
	, debugUtilsMessenger( nullptr )
	, surface( nullptr )
	, physicalDevice( nullptr )
	, device( nullptr )
	, swapChain( nullptr )
	, renderPass( nullptr )
	, pipeline( nullptr )
	, framebuffers()
	, commandPool( nullptr )
	, imageAvailableSemaphores()
	, renderFinishedSemaphores()
	, frameFences()
	, graphicsQueue( nullptr )
	, presentQueue( nullptr )
	, descriptorLayout( nullptr )
	, descriptorPool( nullptr )
	, descriptorSets()
	, current_frame( 0 )
	, window( window )
	, frameBufferSize( window.getFrameBufferSize() )

	, vertexBuffer( nullptr )
	, vbDeviceMemory( nullptr )
	, indexBuffer( nullptr )
	, ibDeviceMemory( nullptr )

	, textureImage( nullptr )
	, textureImageView( nullptr )
	, textureDeviceMemory( nullptr )
{}

v3d::vulkan::Context::~Context()
{
	release();
}

bool v3d::vulkan::Context::init( const v3d::glfw::Window& window, const bool enableValidationLayer )
{
	// Init logger
	auto& logger = v3d::Logger::getInstance();

	// Get version
	uint32_t major, minor, patch;
	if (!vulkan::utils::getVersion( major, minor, patch )) { logger.critical( "Failed to get Context version." ); return false; }
	logger.trace( "Context version: " + std::to_string( major ) + "." + std::to_string( minor ) + "." + std::to_string( patch ) );

	validationLayerEnabled = enableValidationLayer;

	// temp
	auto& vertices = vertexData.getVertexData();
	//vertices.push_back( v3d::V3_C4( { -0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } ) );
	//vertices.push_back( v3d::V3_C4( { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } ) );
	//vertices.push_back( v3d::V3_C4( { 0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } ) );
	//vertices.push_back( v3d::V3_C4( { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } ) );

	//vertices.push_back( v3d::V3_C4_T2( { -0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } ) );
	//vertices.push_back( v3d::V3_C4_T2( { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } ) );
	//vertices.push_back( v3d::V3_C4_T2( { 0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } ) );
	//vertices.push_back( v3d::V3_C4_T2( { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } ) );

	const glm::vec4 white( 1 );
	const glm::vec4 red( 1, 0, 0, 1 );
	const glm::vec4 green( 0, 1, 0, 1 );
	const glm::vec4 blue( 0, 0, 1, 1 );
	vertices.push_back( v3d::V3_C4_T2( { -0.5f, 0.5f, 0.0f }, white, { 0.0f, 1.0f } ) );
	vertices.push_back( v3d::V3_C4_T2( { -0.5f, -0.5f, 0.0f }, white, { 0.0f, 0.0f } ) );
	vertices.push_back( v3d::V3_C4_T2( { 0.5f, 0.5f, 0.0f }, white, { 1.0f, 1.0f } ) );
	vertices.push_back( v3d::V3_C4_T2( { 0.5f, -0.5f, 0.0f }, white, { 1.0f, 0.0f } ) );

	auto& indices = indexData.getVertexData();
	indices = std::vector<uint16_t>( { 0,1,2,3,2,1 } );
	// temp

	if (!initInstance( window )) return false;
	if (validationLayerEnabled) if (!initDebugReport() || !initDebugUtilsMessenger()) return false;
	if (!initSurface( window )) return false;
	if (!initPhysicalDevice()) return false;
	if (!initDevice()) return false;
	if (!initQueue()) return false;
	if (!initSwapChain()) return false;
	if (!initSwapChainImages()) return false;
	if (!initRenderPass()) return false;
	if (!initDescriptorLayout()) return false;
	if (!initGraphicsPipeline()) return false;
	if (!initFrameBuffer()) return false;
	if (!initCommandPool()) return false;
	createTextureImage();
	createTextureImageView();
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffer();
	if (!initDescriptorPool()) return false;
	if (!initDescriptorSet()) return false;
	if (!initSemaphore()) return false;
	if (!initFences()) return false;

	if (!initCommandBuffer()) return false;

	return true;
}

bool v3d::vulkan::Context::initInstance( const v3d::glfw::Window& window )
{
	instance = new v3d::vulkan::Instance();
	if (!instance->init( window, validationLayerEnabled )) return false;
	return true;
}

bool v3d::vulkan::Context::initDebugReport()
{
	debugReportCallback = new v3d::vulkan::DebugReportCallback();
	if (!debugReportCallback->init( *instance )) return false;
	return true;
}

bool v3d::vulkan::Context::initDebugUtilsMessenger()
{
	debugUtilsMessenger = new v3d::vulkan::DebugUtilsMessenger();
	if (!debugUtilsMessenger->init( *instance )) return false;
	return true;
}

bool v3d::vulkan::Context::initSurface( const v3d::glfw::Window& window )
{
	VkSurfaceKHR cVkSurfaceKHR;
	if (!window.createWindowSurface( *instance, cVkSurfaceKHR )) return false;
	surface = vk::SurfaceKHR( cVkSurfaceKHR );
	return true;
}

bool v3d::vulkan::Context::initPhysicalDevice()
{
	physicalDevice = new v3d::vulkan::PhysicalDevice();
	if (!physicalDevice->init( *instance, surface )) return false;
	return true;
}

bool v3d::vulkan::Context::initDevice()
{
	auto graphicsQueueFamilyIndex = physicalDevice->getGraphicsQueueFamilyIndex();

	const float queuePriority = 1.0f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo = vk::DeviceQueueCreateInfo
	(
		{},
		graphicsQueueFamilyIndex,
		1,
		&queuePriority
	);

	std::vector<vk::ExtensionProperties> extensions = physicalDevice->EnumerateDeviceExtensionProperties();

#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().logExtensions( extensions );
#endif
	std::vector<const char*> requiredExtension = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	if (!vulkan::utils::checkExtensionProperties( extensions, requiredExtension )) return false;

	vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo
	(
		{},
		1,
		&deviceQueueCreateInfo,
		0,
		nullptr,
		uint32_t( requiredExtension.size() ),
		requiredExtension.data()
	);

	device = physicalDevice->createDeviceUnique( createInfo );

	return true;
}

bool v3d::vulkan::Context::initSwapChain()
{
	swapChain = new v3d::vulkan::SwapChain();
	if (!swapChain->init( *physicalDevice, device, surface, window )) return false;
	return true;
}

bool v3d::vulkan::Context::initSwapChainImages()
{
	images = device.getSwapchainImagesKHR( swapChain->get() );

	imageViews.reserve( images.size() );
	vk::ComponentMapping componentMapping( vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA );
	vk::ImageSubresourceRange subResourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 );
	for (auto image : images)
	{
		imageViews.push_back( createImageView( image, swapChain->getFormat() ) );
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

	vk::AttachmentReference colorAttachment( 0, vk::ImageLayout::eColorAttachmentOptimal );

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

	renderPass = device.createRenderPass( createInfo );

	return true;
}

bool v3d::vulkan::Context::initGraphicsPipeline()
{
	pipeline = new v3d::vulkan::Pipeline();
	if (!pipeline->init( device, *swapChain, renderPass, descriptorLayout )) return false;
	return true;
}

bool v3d::vulkan::Context::initFrameBuffer()
{
	const std::size_t size = imageViews.size();
	const auto& extent = swapChain->getExtent2D();

	framebuffers.resize( size );

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

		framebuffers[i] = device.createFramebuffer( createInfo );
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

	commandPool = device.createCommandPool( createInfo );

	return true;
}

bool v3d::vulkan::Context::initSemaphore()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		imageAvailableSemaphores.push_back( device.createSemaphore( vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() ) ) );
		renderFinishedSemaphores.push_back( device.createSemaphore( vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() ) ) );
	}

	return true;
}

bool v3d::vulkan::Context::initFences()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		frameFences.push_back( device.createFence( vk::FenceCreateInfo( vk::FenceCreateFlags( vk::FenceCreateFlagBits::eSignaled ) ) ) );
	}

	return true;
}

bool v3d::vulkan::Context::initQueue()
{
	graphicsQueue = new v3d::vulkan::Queue();
	if (!graphicsQueue->init( device, physicalDevice->getGraphicsQueueFamilyIndex() )) return false;

	presentQueue = new v3d::vulkan::Queue();
	if (!presentQueue->init( device, physicalDevice->getPresentQueueFamilyIndex() )) return false;
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

	const std::vector<vk::CommandBuffer> cbs = device.allocateCommandBuffers( allocInfo );

	for (std::size_t i = 0; i < fbSize; i++)
	{
		auto newCB = new v3d::vulkan::CommandBuffer( cbs[i] );
		commandBuffers.push_back( newCB );
		newCB->begin( vk::CommandBufferUsageFlagBits::eSimultaneousUse );
		newCB->record( framebuffers[i], renderPass, *swapChain, *pipeline, vertexBuffer, indexBuffer, static_cast<uint32_t>(indexData.getSize()), descriptorSets[i] );
		newCB->end();
	}

	return true;
}

bool v3d::vulkan::Context::initDescriptorLayout()
{
	vk::DescriptorSetLayoutBinding uboLayoutBinding
	(
		0,
		vk::DescriptorType::eUniformBuffer,
		1,
		vk::ShaderStageFlagBits::eVertex
	);

	vk::DescriptorSetLayoutBinding samplerBinding
	(
		1,
		vk::DescriptorType::eCombinedImageSampler,
		1,
		vk::ShaderStageFlagBits::eFragment
	);

	vk::DescriptorSetLayoutBinding bindings[2] = { uboLayoutBinding, samplerBinding };

	vk::DescriptorSetLayoutCreateInfo layoutInfo
	(
		vk::DescriptorSetLayoutCreateFlags(),
		2, 
		bindings
	);

	descriptorLayout = device.createDescriptorSetLayout( layoutInfo );

	return true;
}

bool v3d::vulkan::Context::initDescriptorPool()
{
	vk::DescriptorPoolSize uboPoolSize
	(
		vk::DescriptorType::eUniformBuffer,
		static_cast<uint32_t>(images.size())
	);

	vk::DescriptorPoolSize samplerPoolSize
	(
		vk::DescriptorType::eCombinedImageSampler,
		static_cast<uint32_t>(images.size())
	);

	vk::DescriptorPoolSize poolSizes[2] = { uboPoolSize, samplerPoolSize };

	vk::DescriptorPoolCreateInfo poolInfo
	(
		vk::DescriptorPoolCreateFlags(),
		static_cast<uint32_t>(images.size()),
		2,
		poolSizes
	);

	descriptorPool = device.createDescriptorPool( poolInfo );

	return true;
}

bool v3d::vulkan::Context::initDescriptorSet()
{
	const std::size_t size = images.size();

	std::vector<vk::DescriptorSetLayout> layouts( size, descriptorLayout );
	vk::DescriptorSetAllocateInfo allocInfo
	(
		descriptorPool,
		static_cast<uint32_t>(size),
		layouts.data()
	);

	descriptorSets = device.allocateDescriptorSets( allocInfo );

	for (std::size_t i = 0; i < size; i++)
	{
		vk::DescriptorBufferInfo bufferInfo
		(
			uniformBuffers[i],
			vk::DeviceSize( 0 ),
			vk::DeviceSize( sizeof( glm::mat4 ) * 3 )
		);

		vk::DescriptorImageInfo imageInfo
		(
			textureSampler,
			textureImageView,
			vk::ImageLayout::eShaderReadOnlyOptimal
		);

		vk::WriteDescriptorSet uboDescriptorWrite
		(
			descriptorSets[i],
			0, 0,
			1,
			vk::DescriptorType::eUniformBuffer,
			nullptr,
			&bufferInfo,
			nullptr
		);

		vk::WriteDescriptorSet samplerDescriptorWrite
		(
			descriptorSets[i],
			1, 0,
			1,
			vk::DescriptorType::eCombinedImageSampler,
			&imageInfo,
			nullptr,
			nullptr
		);

		const vk::WriteDescriptorSet descriptorWrites[2] = { uboDescriptorWrite, samplerDescriptorWrite };

		device.updateDescriptorSets( 2, descriptorWrites, 0, nullptr );
	}

	return true;
}

bool v3d::vulkan::Context::recreateSwapChain()
{
	device.waitIdle();

	releaseSwapChain();

	if (!initSwapChain()) return false;
	if (!initSwapChainImages()) return false;
	if (!initRenderPass()) return false;
	if (!initDescriptorLayout()) return false;
	if (!initGraphicsPipeline()) return false;
	if (!initFrameBuffer()) return false;
	if (!initCommandPool()) return false;
	createUniformBuffer();
	if (!initDescriptorPool()) return false;
	if (!initDescriptorSet()) return false;
	if (!initCommandBuffer()) return false;

	v3d::Logger::getInstance().info( "Recreated swapchain" );

	return true;
}

v3d::vulkan::CommandBuffer v3d::vulkan::Context::createCommandBuffer( const vk::CommandBufferLevel level )
{
	const vk::CommandBufferAllocateInfo allocInfo( commandPool, level, 1 );
	return v3d::vulkan::CommandBuffer( device.allocateCommandBuffers( allocInfo ).front() );
}

vk::Buffer v3d::vulkan::Context::createBuffer( const uint64_t size, const vk::BufferUsageFlags usageFlags ) const
{
	vk::BufferCreateInfo createInfo
	(
		vk::BufferCreateFlags(),
		size,
		usageFlags
	);

	return device.createBuffer( createInfo );
}

vk::DeviceMemory v3d::vulkan::Context::createDeviceMemory( const vk::Buffer& buffer, const vk::MemoryPropertyFlags memoryPropertyFlags ) const
{
	const vk::MemoryRequirements memRequirements = device.getBufferMemoryRequirements( buffer );
	const vk::MemoryAllocateInfo allocInfo
	(
		memRequirements.size,
		physicalDevice->getMemoryTypeIndex( memRequirements.memoryTypeBits, memoryPropertyFlags )
	);

	vk::DeviceMemory deviceMemory = device.allocateMemory( allocInfo );
	device.bindBufferMemory( buffer, deviceMemory, vk::DeviceSize( 0 ) );
	return deviceMemory;
}

void v3d::vulkan::Context::copyBuffer( const vk::Buffer& src, const vk::Buffer& dst, const vk::DeviceSize size )
{
	auto cb = createCommandBuffer();
	cb.begin( vk::CommandBufferUsageFlagBits::eOneTimeSubmit );
	cb.copyBuffer( src, dst, size );
	cb.end();
	oneTimeSubmit( cb );

	device.freeCommandBuffers( commandPool, cb.getHandle() );
}

void v3d::vulkan::Context::createVertexBuffer()
{
	vertexBuffer = createBuffer( vertexData.getDataSize(), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer );
	vbDeviceMemory = createDeviceMemory( vertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal );

	vk::Buffer stagingBuffer = createBuffer( vertexData.getDataSize(), vk::BufferUsageFlagBits::eTransferSrc );
	vk::DeviceMemory stagingDeviceMemory = createDeviceMemory( stagingBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

	void* data = device.mapMemory( stagingDeviceMemory, 0, vertexData.getDataSize() );
	memcpy( data, vertexData.getData(), vertexData.getDataSize() );
	device.unmapMemory( stagingDeviceMemory );

	copyBuffer( stagingBuffer, vertexBuffer, vertexData.getDataSize() );

	device.destroyBuffer( stagingBuffer );
	device.freeMemory( stagingDeviceMemory );
}

void v3d::vulkan::Context::createIndexBuffer()
{
	indexBuffer = createBuffer( indexData.getDataSize(), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer );
	ibDeviceMemory = createDeviceMemory( indexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal );

	vk::Buffer stagingBuffer = createBuffer( indexData.getDataSize(), vk::BufferUsageFlagBits::eTransferSrc );
	vk::DeviceMemory stagingDeviceMemory = createDeviceMemory( stagingBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

	void* data = device.mapMemory( stagingDeviceMemory, 0, indexData.getDataSize() );
	memcpy( data, indexData.getData(), indexData.getDataSize() );
	device.unmapMemory( stagingDeviceMemory );

	copyBuffer( stagingBuffer, indexBuffer, indexData.getDataSize() );

	device.destroyBuffer( stagingBuffer );
	device.freeMemory( stagingDeviceMemory );
}

void v3d::vulkan::Context::createUniformBuffer()
{
	const vk::DeviceSize bufferSize = sizeof( glm::mat4 ) * 3;
	const std::size_t size = imageViews.size();

	uniformBuffers.resize( size );
	ubDeviceMemories.resize( size );

	for (std::size_t i = 0; i < size; i++)
	{
		uniformBuffers.at( i ) = createBuffer( bufferSize, vk::BufferUsageFlagBits::eUniformBuffer );
		ubDeviceMemories.at( i ) = createDeviceMemory( uniformBuffers.at( i ), vk::MemoryPropertyFlagBits::eHostCoherent );
	}
}

void v3d::vulkan::Context::updateUniformBuffer( const uint32_t imageIndex )
{
	static struct UniformBufferObject { glm::mat4 m, v, p; } ubo;

	ubo.m = glm::translate( glm::scale( glm::mat4( 1 ), glm::vec3( 0.5, 0.5, 1 ) ), glm::vec3( 5, 0.0, 0.0 ) );
	ubo.m = glm::scale( glm::mat4( 1 ), glm::vec3( 5, 5, 1 ) );

	static struct Camera
	{
		glm::vec3 pos;
		glm::vec3 rot;
	} cam;

	cam.pos = glm::vec3( 0, 2, -10 );
	ubo.v = glm::translate( glm::mat4( 1 ), glm::vec3( cam.pos.x, cam.pos.y, cam.pos.z ) );

	const float fovy = 70.0f;
	const auto& extent = swapChain->getExtent2D();
	const float aspect = static_cast<float>(extent.width) / static_cast<float>(extent.height);
	const float nears = 0.1f;
	const float fars = 1000.0f;
	ubo.p = glm::perspective( glm::radians( fovy ), aspect, nears, fars );

	void* data = device.mapMemory( ubDeviceMemories[imageIndex], 0, sizeof( ubo ) );
	memcpy( data, &ubo, sizeof( ubo ) );
	device.unmapMemory( ubDeviceMemories[imageIndex] );
}

void v3d::vulkan::Context::createTextureImage()
{
	lena = v3d::Image::createPNG( "Textures/lena.png" );

	vk::Buffer stagingBuffer = createBuffer( lena->getSize(), vk::BufferUsageFlagBits::eTransferSrc );
	vk::DeviceMemory stagingBufferMemory = createDeviceMemory( stagingBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

	void* data = device.mapMemory( stagingBufferMemory, 0, lena->getSize() );
	memcpy( data, lena->getData(), lena->getSize() );
	device.unmapMemory( stagingBufferMemory );

	createImage( lena->getWidth(), lena->getHeight(), vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal, textureImage, textureDeviceMemory );

	transitionImageLayout( textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );
	copyBufferToImage( stagingBuffer, textureImage, uint32_t( lena->getWidth() ), uint32_t( lena->getHeight() ) );
	transitionImageLayout( textureImage, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal );

	device.destroyBuffer( stagingBuffer );
	device.freeMemory( stagingBufferMemory );
}

void v3d::vulkan::Context::createTextureImageView()
{
	textureImageView = createImageView( textureImage, vk::Format::eR8G8B8A8Unorm );

	vk::SamplerCreateInfo createInfo
	(
		vk::SamplerCreateFlags(),
		vk::Filter::eLinear,
		vk::Filter::eLinear,
		vk::SamplerMipmapMode::eLinear,
		vk::SamplerAddressMode::eMirroredRepeat,
		vk::SamplerAddressMode::eMirroredRepeat,
		vk::SamplerAddressMode::eMirroredRepeat,
		0.0f,
		false,
		1,
		false,
		vk::CompareOp::eAlways,
		0.0f,
		0.0f,
		vk::BorderColor::eIntOpaqueBlack,
		false
	);

	textureSampler = device.createSampler( createInfo, nullptr );
}

void v3d::vulkan::Context::createImage( const std::size_t w, const std::size_t h, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags, const vk::MemoryPropertyFlags memoryPropertyFlags, vk::Image& image, vk::DeviceMemory& deviceMemory )
{
	vk::ImageCreateInfo createInfo
	(
		vk::ImageCreateFlags(),
		vk::ImageType::e2D,
		format,
		vk::Extent3D( w, h, 1 ),
		1u,
		1u,
		vk::SampleCountFlagBits::e1,
		tilling,
		usageFlags
	);

	image = device.createImage( createInfo );
	vk::MemoryRequirements memRequirements = device.getImageMemoryRequirements( image );

	vk::MemoryAllocateInfo allocInfo
	(
		memRequirements.size,
		physicalDevice->getMemoryTypeIndex( memRequirements.memoryTypeBits, memoryPropertyFlags )
	);

	deviceMemory = device.allocateMemory( allocInfo );
	device.bindImageMemory( image, deviceMemory, 0 );
}

void v3d::vulkan::Context::transitionImageLayout( vk::Image& image, const vk::Format& format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout )
{
	auto cb = createCommandBuffer();

	vk::ImageSubresourceRange subresourceRange
	(
		vk::ImageAspectFlagBits::eColor,
		0, 1,
		0, 1
	);

	vk::ImageMemoryBarrier barrier
	(
		vk::AccessFlags(),
		vk::AccessFlags(),
		oldLayout,
		newLayout,
		VK_QUEUE_FAMILY_IGNORED,
		VK_QUEUE_FAMILY_IGNORED,
		image,
		subresourceRange
	);

	vk::PipelineStageFlags srcStage;
	vk::PipelineStageFlags dstStage;

	if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlags();
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

		srcStage = vk::PipelineStageFlagBits::eTopOfPipe;
		dstStage = vk::PipelineStageFlagBits::eTransfer;
	}
	else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
	{
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

		srcStage = vk::PipelineStageFlagBits::eTransfer;
		dstStage = vk::PipelineStageFlagBits::eFragmentShader;
	}
	else
	{
		throw std::invalid_argument( "Unsupported layout transition" );
	}

	cb.begin( vk::CommandBufferUsageFlagBits::eOneTimeSubmit );
	cb.commandBuffer.pipelineBarrier( srcStage, dstStage, vk::DependencyFlagBits::eByRegion, 0, nullptr, 0, nullptr, 1, &barrier );
	cb.end();
	oneTimeSubmit( cb );
}

void v3d::vulkan::Context::copyBufferToImage( vk::Buffer& buffer, vk::Image& dst, const uint32_t width, const uint32_t height )
{
	auto cb = createCommandBuffer();

	vk::ImageSubresourceLayers imgSubresourceLayer
	(
		vk::ImageAspectFlagBits::eColor,
		0, 0, 1
	);

	vk::BufferImageCopy region
	(
		0, 0, 0,
		imgSubresourceLayer,
		vk::Offset3D( 0, 0, 0 ),
		vk::Extent3D( width, height, 1 )
	);

	cb.begin( vk::CommandBufferUsageFlagBits::eOneTimeSubmit );
	cb.commandBuffer.copyBufferToImage( buffer, dst, vk::ImageLayout::eTransferDstOptimal, 1, &region );
	cb.end();
	oneTimeSubmit( cb );
}

vk::ImageView v3d::vulkan::Context::createImageView( vk::Image& image, const vk::Format& format )
{
	vk::ImageViewCreateInfo createInfo
	(
		vk::ImageViewCreateFlags(),
		image,
		vk::ImageViewType::e2D,
		format,
		vk::ComponentMapping( vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA ),
		vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 )
	);

	return device.createImageView( createInfo, nullptr );
}

void v3d::vulkan::Context::oneTimeSubmit( v3d::vulkan::CommandBuffer& cb )
{
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cb.commandBuffer;

	graphicsQueue->submit( submitInfo );
	graphicsQueue->waitIdle();
}

void v3d::vulkan::Context::render()
{
	device.waitForFences( 1, &frameFences[current_frame], true, std::numeric_limits<uint64_t>::max() );

	const vk::ResultValue<uint32_t> result = device.acquireNextImageKHR( swapChain->get(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[current_frame], nullptr );
	if (result.result == vk::Result::eErrorOutOfDateKHR)
	{
		recreateSwapChain();
		return;
	}
	else if (result.result != vk::Result::eSuccess && result.result != vk::Result::eSuboptimalKHR)
	{
		throw std::runtime_error( "failed to acquire swap chain image!" );
	}
	assert( result.value < framebuffers.size() );

	updateUniformBuffer( result.value );

	vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[current_frame] };
	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[current_frame] };
	vk::PipelineStageFlags waitFlags[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::CommandBuffer commandBuffers[] = { this->commandBuffers.at( result.value )->getHandle() };
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

	device.resetFences( frameFences[current_frame] );

	graphicsQueue->submit( submitInfo, frameFences[current_frame] );

	vk::SwapchainKHR swapChains[] = { swapChain->get() };
	vk::PresentInfoKHR presentInfo
	(
		1,
		signalSemaphores,
		1,
		swapChains,
		&result.value
	);

	const vk::Result presentResult = presentQueue->present( presentInfo );

	if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || frameBufferSize != window.getFrameBufferSize())
	{
		frameBufferSize = window.getFrameBufferSize();
		recreateSwapChain();
	}
	else if (presentResult != vk::Result::eSuccess)
	{
		throw std::runtime_error( "Failed to present swap chain image" );
	}

	current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void v3d::vulkan::Context::waitIdle()
{
	device.waitIdle();
}

const v3d::vulkan::Instance& v3d::vulkan::Context::getInstance() const
{
	return *instance;
}

void v3d::vulkan::Context::release()
{
	auto& logger = v3d::Logger::getInstance();
	logger.info( "Releasing Context..." );
	SAFE_DELETE( lena );
	device.destroySampler( textureSampler );
	device.destroyImageView( textureImageView );
	device.destroyImage( textureImage );
	device.freeMemory( textureDeviceMemory );
	device.destroyBuffer( vertexBuffer );
	device.freeMemory( vbDeviceMemory );
	device.destroyBuffer( indexBuffer );
	device.freeMemory( ibDeviceMemory );
	for (auto& f : frameFences) { device.destroyFence( f ); }
	frameFences.clear();
	SAFE_DELETE( graphicsQueue );
	SAFE_DELETE( presentQueue );
	for (auto& s : imageAvailableSemaphores) { device.destroySemaphore( s ); }
	imageAvailableSemaphores.clear();
	for (auto& s : renderFinishedSemaphores) { device.destroySemaphore( s ); }
	renderFinishedSemaphores.clear();
	releaseSwapChain();
	device.destroy();
	SAFE_DELETE( physicalDevice );
	instance->get().destroySurfaceKHR( surface );
	SAFE_DELETE( debugUtilsMessenger );
	SAFE_DELETE( debugReportCallback );
	SAFE_DELETE( instance );
	logger.info( "Releasing Context finished" );
}

void v3d::vulkan::Context::releaseSwapChain()
{
	for (std::size_t i = 0; i < images.size(); i++)
	{
		device.destroyBuffer( uniformBuffers.at( i ) );
		device.freeMemory( ubDeviceMemories.at( i ) );
	}
	uniformBuffers.clear();
	ubDeviceMemories.clear();
	device.destroyDescriptorSetLayout( descriptorLayout );
	device.destroyDescriptorPool( descriptorPool );
	for (auto& cb : commandBuffers) { device.freeCommandBuffers( commandPool, cb->getHandle() ); SAFE_DELETE( cb ); }
	commandBuffers.clear();
	device.destroyCommandPool( commandPool );
	for (auto& f : framebuffers) { device.destroyFramebuffer( f ); }
	framebuffers.clear();
	SAFE_DELETE( pipeline );
	device.destroyRenderPass( renderPass );
	for (auto& imageView : imageViews) { device.destroyImageView( imageView ); }
	imageViews.clear();
	images.clear();
	SAFE_DELETE( swapChain );
}