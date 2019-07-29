﻿/**
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
#include "Devices.h"
#include "SwapChain.h"
#include "ShaderModule.h"
#include "Pipeline.h"
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
	, surface()
	, devices( nullptr )
	, physicalDevice()
	, logicalDevice()
	, swapChain( nullptr )
	, renderPass( nullptr )
	, pipeline( nullptr )
	, framebuffers()
	, commandPool( nullptr )
	, imageAvailableSemaphores()
	, renderFinishedSemaphores()
	, frameFences()
	, graphicsQueue()
	, presentQueue()
	, descriptorLayout( nullptr )
	, descriptorPool( nullptr )
	, descriptorSets()
	, current_frame( 0 )
	, window( window )
	, frameBufferSize( window.getFrameBufferSize() )

	, lenaBuffer()

	, lena()
	, RGBW()

	, mvpUBO()
	, dissolveUBO()
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
	
	if (!initInstance( window )) return false;
	if (validationLayerEnabled) if (!initDebugReport() || !initDebugUtilsMessenger()) return false;
	if (!initSurface( window )) return false;
	//if (!initPhysicalDevice()) return false;
	//if (!initDevice()) return false;
	if (!initDevices()) return false;
	if (!initQueue()) return false;
	if (!initSwapChain()) return false;
	if (!initSwapChainImages()) return false;
	if (!initRenderPass()) return false;
	if (!initDescriptorLayout()) return false;
	if (!initGraphicsPipeline()) return false;
	if (!initFrameBuffer()) return false;
	if (!initCommandPool()) return false;
	createTexture( "Textures/lena.png", lena );
	createTexture( "Textures/heightmap.png", RGBW );

	// temp
	const glm::vec4 white( 1 );
	const glm::vec4 red( 1, 0, 0, 1 );
	const glm::vec4 green( 0, 1, 0, 1 );
	const glm::vec4 blue( 0, 0, 1, 1 );

	const float halfWidth = float( lena.imageSource->getWidth() ) * 0.5f;
	const float halfHeight = float( lena.imageSource->getHeight() ) * 0.5f;

	auto& vertices = lenaBuffer.vertexData.getVertexData();
	vertices.push_back( v3d::V3_C4_T2( { -halfWidth, halfHeight, 0.0f }, white, { 0.0f, 1.0f } ) );
	vertices.push_back( v3d::V3_C4_T2( { -halfWidth, -halfHeight, 0.0f }, white, { 0.0f, 0.0f } ) );
	vertices.push_back( v3d::V3_C4_T2( { halfWidth, halfHeight, 0.0f }, white, { 1.0f, 1.0f } ) );
	vertices.push_back( v3d::V3_C4_T2( { halfWidth, -halfHeight, 0.0f }, white, { 1.0f, 0.0f } ) );
	//vertices.push_back( v3d::V3_C4_T2( glm::vec3(-halfWidth, halfHeight, 0.0f) + glm::vec3(256.0f,0.0f,0.0f), white, { 0.0f, 1.0f } ) );
	//vertices.push_back( v3d::V3_C4_T2( glm::vec3(-halfWidth, -halfHeight, 0.0f) + glm::vec3(256.0f,0.0f,0.0f), white, { 0.0f, 0.0f } ) );
	//vertices.push_back( v3d::V3_C4_T2( glm::vec3(halfWidth, halfHeight, 0.0f) + glm::vec3(256.0f,0.0f,0.0f), white, { 1.0f, 1.0f } ) );
	//vertices.push_back( v3d::V3_C4_T2( glm::vec3(halfWidth, -halfHeight, 0.0f) + glm::vec3(256.0f,0.0f,0.0f), white, { 1.0f, 0.0f } ) );

	auto& indices = lenaBuffer.indexData.getVertexData();
	indices = std::vector<uint16_t>( { 0,1,2,3,2,1,  /*4,5,6,7,6,5*/ } );

	createVertexBuffer( lenaBuffer.vertexBuffer, lenaBuffer.vbDeviceMemory, lenaBuffer.vertexData.getDataSize(), lenaBuffer.vertexData.getData() );
	createIndexBuffer( lenaBuffer.indexBuffer, lenaBuffer.ibDeviceMemory, lenaBuffer.indexData.getDataSize(), lenaBuffer.indexData.getData() );
	createUniformBuffer( mvpUBO, sizeof( glm::mat4 ) * 3 );
	//createUniformBuffer( dissolveUBO, sizeof( float ) + sizeof( glm::vec3 ) );
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

bool v3d::vulkan::Context::initDevices()
{
	devices = new v3d::vulkan::Devices();
	if (!devices->initPhysicalDevice( instance->enumeratePhysicalDevices() )) return false;
	if (!devices->initLogicalDevice( surface )) return false;
	physicalDevice = devices->physicalDevice;
	logicalDevice = devices->logicalDevice;
	return true;
}

bool v3d::vulkan::Context::initSwapChain()
{
	swapChain = new v3d::vulkan::SwapChain();
	if (!swapChain->init( physicalDevice, logicalDevice, surface, window )) return false;
	return true;
}

bool v3d::vulkan::Context::initSwapChainImages()
{
	images = logicalDevice.getSwapchainImagesKHR( swapChain->get() );

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

	renderPass = logicalDevice.createRenderPass( createInfo );

	return true;
}

bool v3d::vulkan::Context::initGraphicsPipeline()
{
	pipeline = new v3d::vulkan::Pipeline();
	if (!pipeline->init( logicalDevice, *swapChain, renderPass, descriptorLayout )) return false;
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

		framebuffers[i] = logicalDevice.createFramebuffer( createInfo );
	}

	return true;
}

bool v3d::vulkan::Context::initCommandPool()
{
	const uint32_t graphicsFamilyIndex = devices->getGraphicsQueueFamilyIndex();

	vk::CommandPoolCreateInfo createInfo
	(
		vk::CommandPoolCreateFlags(),
		graphicsFamilyIndex
	);

	commandPool = logicalDevice.createCommandPool( createInfo );

	return true;
}

bool v3d::vulkan::Context::initSemaphore()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		imageAvailableSemaphores.push_back( logicalDevice.createSemaphore( vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() ) ) );
		renderFinishedSemaphores.push_back( logicalDevice.createSemaphore( vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() ) ) );
	}

	return true;
}

bool v3d::vulkan::Context::initFences()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		frameFences.push_back( logicalDevice.createFence( vk::FenceCreateInfo( vk::FenceCreateFlagBits::eSignaled ) ) );
	}

	return true;
}

bool v3d::vulkan::Context::initQueue()
{
	graphicsQueue = logicalDevice.getQueue( devices->getGraphicsQueueFamilyIndex(), 0 );
	presentQueue = logicalDevice.getQueue( devices->getPresentQueueFamilyIndex(), 0 );
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

	const std::vector<vk::CommandBuffer> cbs = logicalDevice.allocateCommandBuffers( allocInfo );

	for (std::size_t i = 0; i < fbSize; i++)
	{
		auto newCB = new v3d::vulkan::CommandBuffer( cbs[i] );
		commandBuffers.push_back( newCB );
		newCB->begin( vk::CommandBufferUsageFlagBits::eSimultaneousUse );
		newCB->record( framebuffers[i], renderPass, *swapChain, *pipeline, lenaBuffer.vertexBuffer, lenaBuffer.indexBuffer, static_cast<uint32_t>(lenaBuffer.indexData.getSize()), descriptorSets[i] );
		newCB->end();
	}

	return true;
}

bool v3d::vulkan::Context::initDescriptorLayout()
{
	vk::DescriptorSetLayoutBinding mvpUBOLayoutBinding
	(
		0,
		vk::DescriptorType::eUniformBuffer,
		1,
		vk::ShaderStageFlagBits::eVertex
	);

	vk::DescriptorSetLayoutBinding lenaSamplerBinding
	(
		1,
		vk::DescriptorType::eCombinedImageSampler,
		1,
		vk::ShaderStageFlagBits::eFragment
	);

	vk::DescriptorSetLayoutBinding RGBWSamplerBinding
	(
		2,
		vk::DescriptorType::eCombinedImageSampler,
		1,
		vk::ShaderStageFlagBits::eFragment
	);

	//vk::DescriptorSetLayoutBinding dissolveUBOLayoutBinding
	//(
	//	3,
	//	vk::DescriptorType::eUniformBuffer,
	//	1,
	//	vk::ShaderStageFlagBits::eFragment
	//);

	//const uint32_t size = 4;
	//vk::DescriptorSetLayoutBinding bindings[size] = { mvpUBOLayoutBinding, lenaSamplerBinding, RGBWSamplerBinding, dissolveUBOLayoutBinding };
	const uint32_t size = 3;
	vk::DescriptorSetLayoutBinding bindings[size] = { mvpUBOLayoutBinding, lenaSamplerBinding, RGBWSamplerBinding };

	vk::DescriptorSetLayoutCreateInfo layoutInfo
	(
		vk::DescriptorSetLayoutCreateFlags(),
		size, 
		bindings
	);

	descriptorLayout = logicalDevice.createDescriptorSetLayout( layoutInfo );

	return true;
}

bool v3d::vulkan::Context::initDescriptorPool()
{
	vk::DescriptorPoolSize uboPoolSize
	(
		vk::DescriptorType::eUniformBuffer,
		static_cast<uint32_t>(images.size())
	);

	vk::DescriptorPoolSize lenaSamplerPoolSize
	(
		vk::DescriptorType::eCombinedImageSampler,
		static_cast<uint32_t>(images.size())
	);

	vk::DescriptorPoolSize RGBWSamplerPoolSize
	(
		vk::DescriptorType::eCombinedImageSampler,
		static_cast<uint32_t>(images.size())
	);

	const uint32_t size = 3;
	vk::DescriptorPoolSize poolSizes[size] = { uboPoolSize, lenaSamplerPoolSize, RGBWSamplerPoolSize };

	vk::DescriptorPoolCreateInfo poolInfo
	(
		vk::DescriptorPoolCreateFlags(),
		static_cast<uint32_t>(images.size()),
		size,
		poolSizes
	);

	descriptorPool = logicalDevice.createDescriptorPool( poolInfo );

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

	descriptorSets = logicalDevice.allocateDescriptorSets( allocInfo );

	for (std::size_t i = 0; i < size; i++)
	{
		vk::DescriptorBufferInfo mvpUBOInfo
		(
			mvpUBO.buffers[i],
			vk::DeviceSize( 0 ),
			vk::DeviceSize( sizeof( glm::mat4 ) * 3 )
		);

		vk::DescriptorImageInfo lenaImageInfo
		(
			lena.sampler,
			lena.imageView,
			vk::ImageLayout::eShaderReadOnlyOptimal
		);

		vk::DescriptorImageInfo RGBWImageInfo
		(
			RGBW.sampler,
			RGBW.imageView,
			vk::ImageLayout::eShaderReadOnlyOptimal
		);

		//vk::DescriptorBufferInfo dissolveUBOInfo
		//{
		//	dissolveUBO.buffers[i],
		//	vk::DeviceSize( 0 ),
		//	vk::DeviceSize( sizeof( float ) + sizeof( glm::vec3 ) )
		//};

		vk::WriteDescriptorSet mvpUBODescriptorWrite
		(
			descriptorSets[i],
			0, 0,
			1,
			vk::DescriptorType::eUniformBuffer,
			nullptr,
			&mvpUBOInfo,
			nullptr
		);

		vk::WriteDescriptorSet lenaSamplerDescriptorWrite
		(
			descriptorSets[i],
			1, 0,
			1,
			vk::DescriptorType::eCombinedImageSampler,
			&lenaImageInfo,
			nullptr,
			nullptr
		);

		vk::WriteDescriptorSet RGBWSamplerDescriptorWrite
		(
			descriptorSets[i],
			2, 0,
			1,
			vk::DescriptorType::eCombinedImageSampler,
			&RGBWImageInfo,
			nullptr,
			nullptr
		);

		//vk::WriteDescriptorSet dissolveUBODescriptorWrite
		//(
		//	descriptorSets[i],
		//	3, 0,
		//	1,
		//	vk::DescriptorType::eUniformBuffer,
		//	nullptr,
		//	&dissolveUBOInfo,
		//	nullptr
		//);

		//const uint32_t size = 4;
		//const vk::WriteDescriptorSet descriptorWrites[size] = { mvpUBODescriptorWrite, lenaSamplerDescriptorWrite, RGBWSamplerDescriptorWrite, dissolveUBODescriptorWrite };
		const uint32_t uboSize = 3;
		const vk::WriteDescriptorSet descriptorWrites[uboSize] = { mvpUBODescriptorWrite, lenaSamplerDescriptorWrite, RGBWSamplerDescriptorWrite };

		logicalDevice.updateDescriptorSets( uboSize, descriptorWrites, 0, nullptr );
	}

	return true;
}

bool v3d::vulkan::Context::recreateSwapChain()
{
	logicalDevice.waitIdle();

	releaseSwapChain();

	if (!initSwapChain()) return false;
	if (!initSwapChainImages()) return false;
	if (!initRenderPass()) return false;
	if (!initDescriptorLayout()) return false;
	if (!initGraphicsPipeline()) return false;
	if (!initFrameBuffer()) return false;
	if (!initCommandPool()) return false;	
	createUniformBuffer( mvpUBO, sizeof( glm::mat4 ) * 3 );
	//createUniformBuffer( dissolveUBO, sizeof( float ) + sizeof( glm::vec3 ) );
	if (!initDescriptorPool()) return false;
	if (!initDescriptorSet()) return false;
	if (!initCommandBuffer()) return false;

	v3d::Logger::getInstance().info( "Recreated swapchain" );

	return true;
}

v3d::vulkan::CommandBuffer v3d::vulkan::Context::createCommandBuffer( const vk::CommandBufferLevel level )
{
	const vk::CommandBufferAllocateInfo allocInfo( commandPool, level, 1 );
	return v3d::vulkan::CommandBuffer( logicalDevice.allocateCommandBuffers( allocInfo ).front() );
}

vk::Buffer v3d::vulkan::Context::createBuffer( const uint64_t size, const vk::BufferUsageFlags usageFlags ) const
{
	vk::BufferCreateInfo createInfo
	(
		vk::BufferCreateFlags(),
		size,
		usageFlags
	);

	return logicalDevice.createBuffer( createInfo );
}

vk::DeviceMemory v3d::vulkan::Context::createDeviceMemory( const vk::Buffer& buffer, const vk::MemoryPropertyFlags memoryPropertyFlags ) const
{
	const vk::MemoryRequirements memRequirements = logicalDevice.getBufferMemoryRequirements( buffer );
	const vk::MemoryAllocateInfo allocInfo
	(
		memRequirements.size,
		devices->getMemoryTypeIndex( memRequirements.memoryTypeBits, memoryPropertyFlags )
	);

	vk::DeviceMemory deviceMemory = logicalDevice.allocateMemory( allocInfo );
	logicalDevice.bindBufferMemory( buffer, deviceMemory, vk::DeviceSize( 0 ) );
	return deviceMemory;
}

void v3d::vulkan::Context::copyBuffer( const vk::Buffer& src, const vk::Buffer& dst, const vk::DeviceSize size )
{
	auto cb = createCommandBuffer();
	cb.begin( vk::CommandBufferUsageFlagBits::eOneTimeSubmit );
	cb.copyBuffer( src, dst, size );
	cb.end();
	oneTimeSubmit( cb );

	logicalDevice.freeCommandBuffers( commandPool, cb.getHandle() );
}

void v3d::vulkan::Context::createVertexBuffer( vk::Buffer& vBuffer, vk::DeviceMemory& vbDeviceMemory, const uint32_t vbDataSize, const void* vbData )
{
	vBuffer = createBuffer( vbDataSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer );
	vbDeviceMemory = createDeviceMemory( vBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal );

	vk::Buffer stagingBuffer = createBuffer( vbDataSize, vk::BufferUsageFlagBits::eTransferSrc );
	vk::DeviceMemory stagingDeviceMemory = createDeviceMemory( stagingBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

	void* data = logicalDevice.mapMemory( stagingDeviceMemory, 0, vbDataSize );
	memcpy( data, vbData, vbDataSize );
	logicalDevice.unmapMemory( stagingDeviceMemory );

	copyBuffer( stagingBuffer, vBuffer, vbDataSize );

	logicalDevice.destroyBuffer( stagingBuffer );
	logicalDevice.freeMemory( stagingDeviceMemory );
}

void v3d::vulkan::Context::createIndexBuffer( vk::Buffer& iBuffer, vk::DeviceMemory& ibDeviceMemory, const uint32_t ibDataSize, const void* ibData )
{
	iBuffer = createBuffer( ibDataSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer );
	ibDeviceMemory = createDeviceMemory( iBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal );

	vk::Buffer stagingBuffer = createBuffer( ibDataSize, vk::BufferUsageFlagBits::eTransferSrc );
	vk::DeviceMemory stagingDeviceMemory = createDeviceMemory( stagingBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

	void* data = logicalDevice.mapMemory( stagingDeviceMemory, 0, ibDataSize );
	memcpy( data, ibData, ibDataSize );
	logicalDevice.unmapMemory( stagingDeviceMemory );

	copyBuffer( stagingBuffer, iBuffer, ibDataSize );

	logicalDevice.destroyBuffer( stagingBuffer );
	logicalDevice.freeMemory( stagingDeviceMemory );
}

void v3d::vulkan::Context::createUniformBuffer( UBO& ubo, const std::size_t uboDataSize )
{
	const std::size_t size = imageViews.size();

	ubo.buffers.resize( size );
	ubo.deviceMemories.resize( size );

	for (std::size_t i = 0; i < size; i++)
	{
		ubo.buffers.at( i ) = createBuffer( uboDataSize, vk::BufferUsageFlagBits::eUniformBuffer );
		ubo.deviceMemories.at( i ) = createDeviceMemory( ubo.buffers.at( i ), vk::MemoryPropertyFlagBits::eHostCoherent );
	}
}

void v3d::vulkan::Context::updateMVPUBO( const uint32_t imageIndex )
{
	static struct UniformBufferObject { glm::mat4 m, v, p; } ubo;

	glm::vec2 screenSize = glm::vec2( 1280, 720 );
	float fovy = 70.0f;
	glm::mat4 screenSpaceMatrix = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, -((screenSize.y * 0.5f) / tanf( glm::radians( fovy * 0.5f ) )) ) );

	ubo.m = glm::translate( glm::mat4( 1 ), glm::vec3( 5, 0.0, 0.0 ) );
	//ubo.m = glm::scale( glm::mat4( 1 ), glm::vec3( 5, 5, 1 ) );
	ubo.m = screenSpaceMatrix;

	static struct Camera
	{
		glm::vec3 pos;
		glm::vec3 rot;
	} cam;

	cam.pos = glm::vec3( 0, 2, -10 );
	ubo.v = glm::translate( glm::mat4( 1 ), glm::vec3( cam.pos.x, cam.pos.y, cam.pos.z ) );

	//const float fovy = 70.0f;
	const auto& extent = swapChain->getExtent2D();
	const float aspect = static_cast<float>(extent.width) / static_cast<float>(extent.height);
	const float nears = 0.1f;
	const float fars = 1000.0f;
	ubo.p = glm::perspective( glm::radians( fovy ), aspect, nears, fars );

	void* data = logicalDevice.mapMemory( mvpUBO.deviceMemories[imageIndex], 0, sizeof( ubo ) );
	memcpy( data, &ubo, sizeof( ubo ) );
	logicalDevice.unmapMemory( mvpUBO.deviceMemories[imageIndex] );
}

void v3d::vulkan::Context::updateDissolveUBO( const uint32_t imageIndex )
{
	static struct UniformBufferObject { glm::vec3 c; float v = 0.0f; } ubo;
	static bool flip = true;

	ubo.c = glm::vec3( 0,0,1 );
	const float d = 1.0f / 8000.0f;
	if (flip) ubo.v += d; else ubo.v -= d;
	if (ubo.v > 1.0f) flip = false;
	else if(ubo.v < 0.0f) flip = true;
	ubo.v = glm::clamp( ubo.v, 0.0f, 1.0f );

	void* data = logicalDevice.mapMemory( dissolveUBO.deviceMemories[imageIndex], 0, sizeof( ubo ) );
	memcpy( data, &ubo, sizeof( ubo ) );
	logicalDevice.unmapMemory( dissolveUBO.deviceMemories[imageIndex] );
}

void v3d::vulkan::Context::createTexture( const char* path, v3d::vulkan::Context::Texture& texture )
{
	createTextureImage( path, texture );
	createTextureImageView( texture );
}

void v3d::vulkan::Context::createTextureImage( const char* path, v3d::vulkan::Context::Texture& texture )
{
	texture.imageSource = v3d::Image::createPNG( path );

	vk::Buffer stagingBuffer = createBuffer( texture.imageSource->getSize(), vk::BufferUsageFlagBits::eTransferSrc );
	vk::DeviceMemory stagingBufferMemory = createDeviceMemory( stagingBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

	void* data = logicalDevice.mapMemory( stagingBufferMemory, 0, texture.imageSource->getSize() );
	memcpy( data, texture.imageSource->getData(), texture.imageSource->getSize() );
	logicalDevice.unmapMemory( stagingBufferMemory );

	createImage( texture.imageSource->getWidth(), texture.imageSource->getHeight(), vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal, texture.image, texture.deviceMemory );

	transitionImageLayout( texture.image, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );
	copyBufferToImage( stagingBuffer, texture.image, uint32_t( texture.imageSource->getWidth() ), uint32_t( texture.imageSource->getHeight() ) );
	transitionImageLayout( texture.image, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal );

	logicalDevice.destroyBuffer( stagingBuffer );
	logicalDevice.freeMemory( stagingBufferMemory );
}

void v3d::vulkan::Context::createTextureImageView( v3d::vulkan::Context::Texture& texture )
{
	texture.imageView = createImageView( texture.image, vk::Format::eR8G8B8A8Unorm );

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

	texture.sampler = logicalDevice.createSampler( createInfo, nullptr );
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

	image = logicalDevice.createImage( createInfo );
	vk::MemoryRequirements memRequirements = logicalDevice.getImageMemoryRequirements( image );

	vk::MemoryAllocateInfo allocInfo
	(
		memRequirements.size,
		devices->getMemoryTypeIndex( memRequirements.memoryTypeBits, memoryPropertyFlags )
	);

	deviceMemory = logicalDevice.allocateMemory( allocInfo );
	logicalDevice.bindImageMemory( image, deviceMemory, 0 );
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

	return logicalDevice.createImageView( createInfo, nullptr );
}

void v3d::vulkan::Context::oneTimeSubmit( v3d::vulkan::CommandBuffer& cb )
{
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cb.commandBuffer;

	graphicsQueue.submit( submitInfo, nullptr );
	graphicsQueue.waitIdle();
}

void v3d::vulkan::Context::render()
{
	logicalDevice.waitForFences( 1, &frameFences[current_frame], true, std::numeric_limits<uint64_t>::max() );

	const vk::ResultValue<uint32_t> result = logicalDevice.acquireNextImageKHR( swapChain->get(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[current_frame], nullptr );
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

	updateMVPUBO( result.value );
	//updateDissolveUBO( result.value );

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

	logicalDevice.resetFences( frameFences[current_frame] );

	graphicsQueue.submit( submitInfo, frameFences[current_frame] );

	vk::SwapchainKHR swapChains[] = { swapChain->get() };
	vk::PresentInfoKHR presentInfo
	(
		1,
		signalSemaphores,
		1,
		swapChains,
		&result.value
	);

	vk::Result presentResult = vk::Result::eSuccess;
	try	
	{
		presentResult = presentQueue.presentKHR( presentInfo );
	}
	catch (vk::OutOfDateKHRError& ood)
	{
		presentResult = vk::Result::eErrorOutOfDateKHR;
	}

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
	logicalDevice.waitIdle();
}

const v3d::vulkan::Instance& v3d::vulkan::Context::getInstance() const
{
	return *instance;
}

void v3d::vulkan::Context::release()
{
	auto& logger = v3d::Logger::getInstance();
	logger.info( "Releasing Context..." );
	SAFE_DELETE( lena.imageSource );
	logicalDevice.destroySampler( lena.sampler );
	logicalDevice.destroyImageView( lena.imageView );
	logicalDevice.destroyImage( lena.image );
	logicalDevice.freeMemory( lena.deviceMemory );
	SAFE_DELETE( RGBW.imageSource );
	logicalDevice.destroySampler( RGBW.sampler );
	logicalDevice.destroyImageView( RGBW.imageView );
	logicalDevice.destroyImage( RGBW.image );
	logicalDevice.freeMemory( RGBW.deviceMemory );
	logicalDevice.destroyBuffer( lenaBuffer.vertexBuffer );
	logicalDevice.freeMemory( lenaBuffer.vbDeviceMemory );
	logicalDevice.destroyBuffer( lenaBuffer.indexBuffer );
	logicalDevice.freeMemory( lenaBuffer.ibDeviceMemory );
	for (auto& f : frameFences) { logicalDevice.destroyFence( f ); }
	frameFences.clear();
	for (auto& s : imageAvailableSemaphores) { logicalDevice.destroySemaphore( s ); }
	imageAvailableSemaphores.clear();
	for (auto& s : renderFinishedSemaphores) { logicalDevice.destroySemaphore( s ); }
	renderFinishedSemaphores.clear();
	releaseSwapChain();
	logicalDevice.destroy();
	SAFE_DELETE( devices );
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
		logicalDevice.destroyBuffer( mvpUBO.buffers.at( i ) );
		logicalDevice.freeMemory( mvpUBO.deviceMemories.at( i ) );
		//logicalDevice.destroyBuffer( dissolveUBO.buffers.at( i ) );
		//logicalDevice.freeMemory( dissolveUBO.deviceMemories.at( i ) );
	}
	mvpUBO.buffers.clear();
	mvpUBO.deviceMemories.clear();
	//dissolveUBO.buffers.clear();
	//dissolveUBO.deviceMemories.clear();
	logicalDevice.destroyDescriptorSetLayout( descriptorLayout );
	logicalDevice.destroyDescriptorPool( descriptorPool );
	for (auto& cb : commandBuffers) { logicalDevice.freeCommandBuffers( commandPool, cb->getHandle() ); SAFE_DELETE( cb ); }
	commandBuffers.clear();
	logicalDevice.destroyCommandPool( commandPool );
	for (auto& f : framebuffers) { logicalDevice.destroyFramebuffer( f ); }
	framebuffers.clear();
	SAFE_DELETE( pipeline );
	logicalDevice.destroyRenderPass( renderPass );
	for (auto& imageView : imageViews) { logicalDevice.destroyImageView( imageView ); }
	imageViews.clear();
	images.clear();
	SAFE_DELETE( swapChain );
}