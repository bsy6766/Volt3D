/**
*	@file Context.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include <glslang/Public/ShaderLang.h>

#include "Context.h"

#include "Engine/Engine.h"
#include "Engine/Window.h"
#include "Instance.h"
#include "Devices/PhysicalDevice.h"
#include "Devices/LogicalDevice.h"
#include "SwapChain.h"
#include "Pipelines/Pipeline.h"
#include "Commands/CommandPool.h"
#include "Commands/CommandBuffer.h"
#include "Buffers/Buffer.h"
#include "Buffers/UniformBuffer.h"
#include "Utils.h"
#include "Config/BuildConfig.h"

#include "Renderer/VertexData.cpp"

#include "Spritesheet/Image.h"
#include "Texture.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Context::Context()
	: instance( nullptr )
	, surface()
	, physicalDevice( nullptr )
	, logicalDevice( nullptr )
	, swapChain( nullptr )
	, renderPass( nullptr )
	, pipeline( nullptr )
	, framebuffers()
	, commandPool( nullptr )
	, imageAvailableSemaphores()
	, renderFinishedSemaphores()
	, frameFences()
	//, descriptorLayout( nullptr )
	, descriptorPool( nullptr )
	, descriptorSets()
	, current_frame( 0 )
	, window( v3d::glfw::Window::get() )
	, frameBufferSize( 0 )

	, lenaBuffer()

	, lena()
{
	frameBufferSize = window->getFrameBufferSize();
}

Context::~Context()
{
	release();
}

v3d::vulkan::Context* Context::get() { return v3d::Engine::get()->getVulkanContext(); }

bool Context::init( const bool enableValidationLayer )
{
	// Init logger
	auto& logger = v3d::Logger::getInstance();

	// Get version
	uint32_t major, minor, patch;
	if (!vulkan::utils::getVersion( major, minor, patch )) { logger.critical( "Failed to get Context version." ); return false; }
	logger.trace( "Context version: " + std::to_string( major ) + "." + std::to_string( minor ) + "." + std::to_string( patch ) );

	// Init shader!
	glslang::InitializeProcess();

	if (!initInstance( enableValidationLayer )) return false;
	if (!initSurface()) return false;
	if (!initPhysicalDevice()) return false;
	if (!initLogicalDevice()) return false;
	if (!initSwapChain()) return false;
	if (!initSwapChainImages()) return false;
	if (!initRenderPass()) return false;
	//if (!initDescriptorLayout()) return false;
	if (!initGraphicsPipeline()) return false;
	if (!initFrameBuffer()) return false;
	if (!initCommandPool()) return false;

	createTexture( "Textures/lena.png", lena );

	// temp
	const glm::vec4 white( 1 );

	const float halfWidth = float( lena.imageSource->getWidth() ) * 0.5f;
	const float halfHeight = float( lena.imageSource->getHeight() ) * 0.5f;

	SAFE_DELETE( lena.imageSource );

	auto& vertices = lenaBuffer.vertexData.getVertexData();
	vertices.push_back( v3d::V3_C4_T2( { -halfWidth, halfHeight, 0.0f }, white, { 0.0f, 1.0f } ) );
	vertices.push_back( v3d::V3_C4_T2( { -halfWidth, -halfHeight, 0.0f }, white, { 0.0f, 0.0f } ) );
	vertices.push_back( v3d::V3_C4_T2( { halfWidth, halfHeight, 0.0f }, white, { 1.0f, 1.0f } ) );
	vertices.push_back( v3d::V3_C4_T2( { halfWidth, -halfHeight, 0.0f }, white, { 1.0f, 0.0f } ) );

	auto& indices = lenaBuffer.indexData.getVertexData();
	indices = std::vector<uint16_t>( { 0,1,2,3,2,1 } );

	createLenaBuffer();
	createMVPUBO();

	if (!initDescriptorPool()) return false;
	if (!initDescriptorSet()) return false;
	if (!initSemaphore()) return false;
	if (!initFences()) return false;

	if (!initCommandBuffer()) return false;

	return true;
}

bool Context::initInstance( const bool enableValidationLayer )
{
	instance = new v3d::vulkan::Instance();
	std::vector<const char*> requiredExtensions;
	window->getGLFWVKExtensions( requiredExtensions );
	if (!instance->init( requiredExtensions, enableValidationLayer )) return false;
	return true;
}

bool Context::initSurface()
{
	VkSurfaceKHR cVkSurfaceKHR;
	if (!window->createWindowSurface( *instance, cVkSurfaceKHR )) return false;
	surface = vk::SurfaceKHR( cVkSurfaceKHR );
	return true;
}

bool Context::initPhysicalDevice()
{
	physicalDevice = new v3d::vulkan::PhysicalDevice();
	return physicalDevice->init( instance->enumeratePhysicalDevices() );
}

bool Context::initLogicalDevice()
{
	logicalDevice = new v3d::vulkan::LogicalDevice();
	return logicalDevice->init( surface, physicalDevice->get() );
}

bool Context::initSwapChain()
{
	swapChain = new v3d::vulkan::SwapChain();
	if (!swapChain->init( physicalDevice->get(), logicalDevice->get(), surface, window->getFrameBufferSize() )) return false;
	return true;
}

bool Context::initSwapChainImages()
{
	images = logicalDevice->get().getSwapchainImagesKHR( swapChain->get() );

	imageViews.reserve( images.size() );
	vk::ComponentMapping componentMapping( vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA );
	vk::ImageSubresourceRange subResourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 );
	for (auto image : images)
	{
		imageViews.push_back( createImageView( image, swapChain->getFormat() ) );
	}

	return true;
}

bool Context::initRenderPass()
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

	renderPass = logicalDevice->get().createRenderPass( createInfo );

	return true;
}

bool Context::initGraphicsPipeline()
{
	pipeline = new v3d::vulkan::Pipeline();
	std::vector<std::filesystem::path> shaderPath;
	shaderPath.push_back( "Shaders/vert.vert" );
	shaderPath.push_back( "Shaders/frag.frag" );
	if (!pipeline->init( shaderPath, swapChain->getExtent2D(), renderPass )) return false;
	return true;
}

bool Context::initFrameBuffer()
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

		framebuffers[i] = logicalDevice->get().createFramebuffer( createInfo );
	}

	return true;
}

bool Context::initCommandPool()
{
	commandPool = new v3d::vulkan::CommandPool( logicalDevice->getGraphicsQueueFamilyIndex() );
	return true;
}

bool Context::initSemaphore()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		imageAvailableSemaphores.push_back( logicalDevice->get().createSemaphore( vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() ) ) );
		renderFinishedSemaphores.push_back( logicalDevice->get().createSemaphore( vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() ) ) );
	}

	return true;
}

bool Context::initFences()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		frameFences.push_back( logicalDevice->get().createFence( vk::FenceCreateInfo( vk::FenceCreateFlagBits::eSignaled ) ) );
	}

	return true;
}

bool Context::initCommandBuffer()
{
	const std::size_t fbSize = framebuffers.size();

	//vk::CommandBufferAllocateInfo allocInfo
	//(
	//	commandPool->get(),
	//	vk::CommandBufferLevel::ePrimary,
	//	static_cast<uint32_t>(fbSize)
	//);

	//const std::vector<vk::CommandBuffer> cbs = logicalDevice->get().allocateCommandBuffers( allocInfo );

	for (std::size_t i = 0; i < fbSize; i++)
	{
		auto newCB = new v3d::vulkan::CommandBuffer( vk::CommandBufferLevel::ePrimary );
		commandBuffers.push_back( newCB );
		newCB->begin( vk::CommandBufferUsageFlagBits::eSimultaneousUse );

		const vk::CommandBuffer& cb = newCB->get();

		//newCB->record( framebuffers[i], renderPass, *swapChain, *pipeline, lenaBuffer.vertexBuffer->getBuffer(), lenaBuffer.indexBuffer->getBuffer(), static_cast<uint32_t>(lenaBuffer.indexData.getSize()), descriptorSets[i] );

		vk::ClearValue clearValue( vk::ClearColorValue( std::array<float, 4>( { 0.2f, 0.2f, 0.2f, 0.2f } ) ) );

		vk::RenderPassBeginInfo renderPassInfo
		(
			renderPass,
			framebuffers[i],
			vk::Rect2D
			(
				vk::Offset2D(),
				swapChain->getExtent2D()
			),
			1,
			&clearValue
		);

		cb.beginRenderPass( renderPassInfo, vk::SubpassContents::eInline );
		cb.bindPipeline( vk::PipelineBindPoint::eGraphics, pipeline->get() );
		cb.setViewport( 0, pipeline->getViewport() );
		cb.setScissor( 0, pipeline->getScissor() );
		vk::DeviceSize offset = 0;
		cb.bindVertexBuffers( 0, lenaBuffer.vertexBuffer->getBuffer(), offset );
		cb.bindIndexBuffer( lenaBuffer.indexBuffer->getBuffer() , offset, vk::IndexType::eUint16 );
		cb.bindDescriptorSets( vk::PipelineBindPoint::eGraphics, pipeline->getLayout(), 0, 1, &descriptorSets[i], 0, nullptr );
		cb.drawIndexed( uint32_t(lenaBuffer.indexData.getSize()), 1, 0, 0, 0 );
		cb.endRenderPass();


		cb.end();
	}

	return true;
}

/*
bool Context::initDescriptorLayout()
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

	const uint32_t size = 2;
	vk::DescriptorSetLayoutBinding bindings[size] = { mvpUBOLayoutBinding, lenaSamplerBinding };

	vk::DescriptorSetLayoutCreateInfo layoutInfo
	(
		vk::DescriptorSetLayoutCreateFlags(),
		size,
		bindings
	);

	descriptorLayout = logicalDevice->get().createDescriptorSetLayout( layoutInfo );

	return true;
}
*/

bool Context::initDescriptorPool()
{
	const uint32_t imageSize = uint32_t( images.size() );

	vk::DescriptorPoolSize uboPoolSize
	(
		vk::DescriptorType::eUniformBuffer,
		imageSize
	);

	vk::DescriptorPoolSize lenaSamplerPoolSize
	(
		vk::DescriptorType::eCombinedImageSampler,
		imageSize
	);

	const uint32_t size = 2;
	vk::DescriptorPoolSize poolSizes[size] = { uboPoolSize, lenaSamplerPoolSize };

	vk::DescriptorPoolCreateInfo poolInfo
	(
		vk::DescriptorPoolCreateFlags(),
		imageSize,
		size,
		poolSizes
	);

	descriptorPool = logicalDevice->get().createDescriptorPool( poolInfo );

	return true;
}

bool Context::initDescriptorSet()
{
	const std::size_t size = images.size();

	std::vector<vk::DescriptorSetLayout> layouts( size, descriptorLayout );
	vk::DescriptorSetAllocateInfo allocInfo
	(
		descriptorPool,
		static_cast<uint32_t>(size),
		layouts.data()
	);

	descriptorSets = logicalDevice->get().allocateDescriptorSets( allocInfo );

	for (std::size_t i = 0; i < size; i++)
	{
		vk::DescriptorBufferInfo mvpUBOInfo
		(
			mvpUBOs[i]->getBuffer(),
			vk::DeviceSize( 0 ),
			vk::DeviceSize( sizeof( glm::mat4 ) * 3 )
		);

		vk::DescriptorImageInfo lenaImageInfo
		(
			lena.sampler,
			lena.imageView,
			vk::ImageLayout::eShaderReadOnlyOptimal
		);

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

		const uint32_t uboSize = 2;
		const vk::WriteDescriptorSet descriptorWrites[uboSize] = { mvpUBODescriptorWrite, lenaSamplerDescriptorWrite };

		logicalDevice->get().updateDescriptorSets( uboSize, descriptorWrites, 0, nullptr );
	}

	return true;
}







bool Context::recreateSwapChain()
{
	logicalDevice->get().waitIdle();

	releaseSwapChain();

	if (!initSwapChain()) return false;
	if (!initSwapChainImages()) return false;
	if (!initRenderPass()) return false;
	//if (!initDescriptorLayout()) return false;
	if (!initGraphicsPipeline()) return false;
	if (!initFrameBuffer()) return false;
	if (!initCommandPool()) return false;
	createMVPUBO();
	if (!initDescriptorPool()) return false;
	if (!initDescriptorSet()) return false;
	if (!initCommandBuffer()) return false;

	v3d::Logger::getInstance().info( "Recreated swapchain" );

	return true;
}

/*
v3d::vulkan::CommandBuffer Context::createCommandBuffer( const vk::CommandBufferLevel level )
{
	const vk::CommandBufferAllocateInfo allocInfo( commandPool, level, 1 );
	return v3d::vulkan::CommandBuffer( logicalDevice->get().allocateCommandBuffers( allocInfo ).front() );
}
*/

void Context::copyBuffer( const vk::Buffer& src, const vk::Buffer& dst, const vk::DeviceSize size )
{
	//auto cb = createCommandBuffer();
	auto oneTimeCB = v3d::vulkan::CommandBuffer( vk::CommandBufferLevel::ePrimary );
	//const vk::CommandBuffer& cb = oneTimeCB.get();
	oneTimeCB.begin();
	oneTimeCB.copyBuffer( src, dst, size );
	oneTimeCB.end();
	oneTimeSubmit( oneTimeCB );

	//logicalDevice->get().freeCommandBuffers( commandPool->get(), oneTimeCB.get() );
}

void Context::createLenaBuffer()
{
	lenaBuffer.vertexBuffer = new v3d::vulkan::Buffer( lenaBuffer.vertexData.getDataSize(), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal );
	lenaBuffer.indexBuffer = new v3d::vulkan::Buffer( lenaBuffer.indexData.getDataSize(), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal );

	const vk::Device& ld = logicalDevice->get();

	{
		v3d::vulkan::Buffer stagingBuffer = v3d::vulkan::Buffer( lenaBuffer.vertexData.getDataSize(), vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

		void* mapPtr = stagingBuffer.mapMemory();
		memcpy( mapPtr, lenaBuffer.vertexData.getData(), lenaBuffer.vertexData.getDataSize() );
		stagingBuffer.unmapMemory();

		copyBuffer( stagingBuffer.getBuffer(), lenaBuffer.vertexBuffer->getBuffer(), lenaBuffer.vertexData.getDataSize() );
	}

	{
		v3d::vulkan::Buffer stagingBuffer = v3d::vulkan::Buffer( lenaBuffer.indexData.getDataSize(), vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

		void* mapPtr = stagingBuffer.mapMemory();
		memcpy( mapPtr, lenaBuffer.indexData.getData(), lenaBuffer.indexData.getDataSize() );
		stagingBuffer.unmapMemory();

		copyBuffer( stagingBuffer.getBuffer(), lenaBuffer.indexBuffer->getBuffer(), lenaBuffer.indexData.getDataSize() );
	}
}

void Context::createMVPUBO()
{
	const std::size_t size = imageViews.size();
	mvpUBOs.resize( size, nullptr );
	mvps.resize( size );

	for (auto& ubo : mvpUBOs)
	{
		ubo = new v3d::vulkan::UniformBuffer( sizeof( MVP ), vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostCoherent );
	}
}

void Context::updateMVPUBO( const uint32_t imageIndex )
{
	glm::vec2 screenSize = glm::vec2( 1280, 720 );
	float fovy = 70.0f;
	glm::mat4 screenSpaceMatrix = glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, -((screenSize.y * 0.5f) / tanf( glm::radians( fovy * 0.5f ) )) ) );

	MVP& curMVP = mvps[imageIndex];

	curMVP.model = glm::translate( glm::mat4( 1 ), glm::vec3( 5, 0.0, 0.0 ) );
	//curMVP.model = glm::scale( glm::mat4( 1 ), glm::vec3( 5, 5, 1 ) );
	curMVP.model = screenSpaceMatrix;

	static struct Camera
	{
		glm::vec3 pos;
		glm::vec3 rot;
	} cam;

	cam.pos = glm::vec3( 0, 2, -10 );
	curMVP.view = glm::translate( glm::mat4( 1 ), glm::vec3( cam.pos.x, cam.pos.y, cam.pos.z ) );

	//const float fovy = 70.0f;
	const auto& extent = swapChain->getExtent2D();
	const float aspect = static_cast<float>(extent.width) / static_cast<float>(extent.height);
	const float nears = 0.1f;
	const float fars = 1000.0f;
	curMVP.projection = glm::perspective( glm::radians( fovy ), aspect, nears, fars );

	mvpUBOs[imageIndex]->update( &curMVP );
}

void Context::createTexture( const char* path, v3d::vulkan::Context::Texture& texture )
{
	createTextureImage( path, texture );
	createTextureImageView( texture );
}

void Context::createTextureImage( const char* path, v3d::vulkan::Context::Texture& texture )
{
	texture.imageSource = v3d::Image::createPNG( path );

	//vk::Buffer stagingBuffer = logicalDevice->get().createBuffer( texture.imageSource->getDataSize(), vk::BufferUsageFlagBits::eTransferSrc );
	//vk::DeviceMemory stagingBufferMemory = logicalDevice->get().createDeviceMemory( stagingBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

	v3d::vulkan::Buffer stagingBuffer = v3d::vulkan::Buffer( texture.imageSource->getDataSize(), vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent );

	void* data = logicalDevice->get().mapMemory( stagingBuffer.getDeviceMemory(), 0, texture.imageSource->getDataSize() );
	memcpy( data, texture.imageSource->getData(), texture.imageSource->getDataSize() );
	logicalDevice->get().unmapMemory( stagingBuffer.getDeviceMemory() );

	createImage( texture.imageSource->getWidth(), texture.imageSource->getHeight(), vk::Format::eR8G8B8A8Unorm, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal, texture.image, texture.deviceMemory );

	transitionImageLayout( texture.image, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal );
	copyBufferToImage( stagingBuffer.getBuffer(), texture.image, uint32_t( texture.imageSource->getWidth() ), uint32_t( texture.imageSource->getHeight() ) );
	transitionImageLayout( texture.image, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal );
}

void Context::createTextureImageView( v3d::vulkan::Context::Texture& texture )
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

	texture.sampler = logicalDevice->get().createSampler( createInfo, nullptr );
}

void Context::createImage( const uint32_t w, const uint32_t h, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags, const vk::MemoryPropertyFlags memoryPropertyFlags, vk::Image& image, vk::DeviceMemory& deviceMemory )
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

	image = logicalDevice->get().createImage( createInfo );
	vk::MemoryRequirements memRequirements = logicalDevice->get().getImageMemoryRequirements( image );

	vk::MemoryAllocateInfo allocInfo
	(
		memRequirements.size,
		physicalDevice->getMemoryTypeIndex( memRequirements.memoryTypeBits, memoryPropertyFlags )
	);

	deviceMemory = logicalDevice->get().allocateMemory( allocInfo );
	logicalDevice->get().bindImageMemory( image, deviceMemory, 0 );
}

void Context::transitionImageLayout( vk::Image& image, const vk::Format& format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout )
{
	auto oneTimeCB = v3d::vulkan::CommandBuffer( vk::CommandBufferLevel::ePrimary );
	//auto cb = createCommandBuffer();

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

	oneTimeCB.begin( vk::CommandBufferUsageFlagBits::eOneTimeSubmit );
	oneTimeCB.get().pipelineBarrier( srcStage, dstStage, vk::DependencyFlagBits::eByRegion, 0, nullptr, 0, nullptr, 1, &barrier );
	oneTimeCB.end();
	oneTimeSubmit( oneTimeCB );
}

void Context::copyBufferToImage( const vk::Buffer& buffer, vk::Image& dst, const uint32_t width, const uint32_t height )
{
	auto oneTimeCB = v3d::vulkan::CommandBuffer( vk::CommandBufferLevel::ePrimary );
	//auto cb = createCommandBuffer();

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

	oneTimeCB.begin( vk::CommandBufferUsageFlagBits::eOneTimeSubmit );
	oneTimeCB.get().copyBufferToImage( buffer, dst, vk::ImageLayout::eTransferDstOptimal, 1, &region );
	oneTimeCB.end();
	oneTimeSubmit( oneTimeCB );
}

vk::ImageView Context::createImageView( vk::Image& image, const vk::Format& format )
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

	return logicalDevice->get().createImageView( createInfo, nullptr );
}

void Context::oneTimeSubmit( v3d::vulkan::CommandBuffer& cb )
{
	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cb.commandBuffer;

	logicalDevice->getGraphicsQueue().submit( submitInfo, nullptr );
	logicalDevice->getGraphicsQueue().waitIdle();
}

void Context::render()
{
	logicalDevice->get().waitForFences( 1, &frameFences[current_frame], true, std::numeric_limits<uint64_t>::max() );

	const vk::ResultValue<uint32_t> result = logicalDevice->get().acquireNextImageKHR( swapChain->get(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[current_frame], nullptr );
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

	vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[current_frame] };
	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[current_frame] };
	vk::PipelineStageFlags waitFlags[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::CommandBuffer commandBuffers[] = { this->commandBuffers.at( result.value )->get() };
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

	logicalDevice->get().resetFences( frameFences[current_frame] );

	logicalDevice->getGraphicsQueue().submit( submitInfo, frameFences[current_frame] );

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
		presentResult = logicalDevice->getPresentQueue().presentKHR( presentInfo );
	}
	catch (vk::OutOfDateKHRError&)
	{
		presentResult = vk::Result::eErrorOutOfDateKHR;
	}

	if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || frameBufferSize != window->getFrameBufferSize())
	{
		frameBufferSize = window->getFrameBufferSize();
		recreateSwapChain();
	}
	else if (presentResult != vk::Result::eSuccess)
	{
		throw std::runtime_error( "Failed to present swap chain image" );
	}

	current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Context::waitIdle()
{
	logicalDevice->get().waitIdle();
}




v3d::vulkan::Instance* Context::getInstance() const
{
	return instance;
}

v3d::vulkan::PhysicalDevice* Context::getPhysicalDevice() const
{
	return physicalDevice;
}

v3d::vulkan::LogicalDevice* Context::getLogicalDevice() const
{
	return logicalDevice;
}

v3d::vulkan::CommandPool* Context::getCommandPool() const
{
	return commandPool;
}

void Context::release()
{
	auto& logger = v3d::Logger::getInstance();
	logger.info( "Releasing Context..." );

	SAFE_DELETE( lena.imageSource );

	logicalDevice->get().destroySampler( lena.sampler );
	logicalDevice->get().destroyImageView( lena.imageView );
	logicalDevice->get().destroyImage( lena.image );
	logicalDevice->get().freeMemory( lena.deviceMemory );

	SAFE_DELETE( lenaBuffer.vertexBuffer );
	SAFE_DELETE( lenaBuffer.indexBuffer );

	for (auto& f : frameFences) { logicalDevice->get().destroyFence( f ); }
	frameFences.clear();

	for (auto& s : imageAvailableSemaphores) { logicalDevice->get().destroySemaphore( s ); }
	imageAvailableSemaphores.clear();
	for (auto& s : renderFinishedSemaphores) { logicalDevice->get().destroySemaphore( s ); }
	renderFinishedSemaphores.clear();

	releaseSwapChain();

	SAFE_DELETE( physicalDevice );
	SAFE_DELETE( logicalDevice );

	instance->get().destroySurfaceKHR( surface );
	SAFE_DELETE( instance );
	logger.info( "Releasing Context finished" );
}

void Context::releaseSwapChain()
{
	const vk::Device& ld = logicalDevice->get();

	for (auto mvpUBO : mvpUBOs) { SAFE_DELETE( mvpUBO ); }
	mvpUBOs.clear();

	//ld.destroyDescriptorSetLayout( descriptorLayout );
	ld.destroyDescriptorPool( descriptorPool );

	for (auto& cb : commandBuffers) SAFE_DELETE( cb );
	commandBuffers.clear();
	SAFE_DELETE( commandPool );

	for (auto& f : framebuffers) { ld.destroyFramebuffer( f ); }
	framebuffers.clear();

	SAFE_DELETE( pipeline );

	ld.destroyRenderPass( renderPass );

	for (auto& imageView : imageViews) { ld.destroyImageView( imageView ); }
	imageViews.clear();
	images.clear();

	SAFE_DELETE( swapChain );
}

VK_NS_END
V3D_NS_END