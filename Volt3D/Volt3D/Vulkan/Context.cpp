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
#include "SwapChain/Framebuffers.h"
#include "SwapChain/Swapchain.h"
#include "SwapChain/RenderPass.h"
#include "Pipelines/Pipeline.h"
#include "Commands/CommandPool.h"
#include "Commands/CommandBuffer.h"
#include "Buffers/Buffer.h"
#include "Buffers/UniformBuffer.h"
#include "Utils.h"
#include "Config/BuildConfig.h"

#include "Renderer/VertexData.cpp"

#include "Spritesheet/Image.h"
#include "Images/Image.h"
#include "Textures/Texture2D.h"


#include "Pipelines/Shader.h"
#include "Pipelines/ShaderState.h"
#include "Pipelines/UniformData.h"
#include "Pipelines/Uniform.h"
#include "Pipelines/UniformBlock.h"
#include "Pipelines/Attribute.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Context::Context()
	: instance( nullptr )
	, surface()
	, physicalDevice( nullptr )
	, logicalDevice( nullptr )
	, swapchain( nullptr )
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

	, lena( nullptr )
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
	if (!initRenderPass()) return false;
	//if (!initDescriptorLayout()) return false;
	if (!initGraphicsPipeline()) return false;
	if (!initFrameBuffer()) return false;
	if (!initCommandPool()) return false;

	//createTexture( "Textures/lena.png", lena );
	lena = v3d::vulkan::Texture2D::create( "Textures/lena.png", vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal );

	// temp
	const glm::vec4 white( 1 );

	const float halfWidth = float( lena->getWidth() ) * 0.5f;
	const float halfHeight = float( lena->getHeight() ) * 0.5f;

	//SAFE_DELETE( lena.imageSource );

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
	return logicalDevice->init( surface, physicalDevice->getVKPhysicalDevice() );
}

bool Context::initSwapChain()
{
	swapchain = new v3d::vulkan::Swapchain();
	if (!swapchain->init()) return false;
	return true;
}

bool Context::initRenderPass()
{
	renderPass = new v3d::vulkan::RenderPass( swapchain->getFormat() );
	return true;
}

bool Context::initGraphicsPipeline()
{
	pipeline = new v3d::vulkan::Pipeline();
	std::vector<std::filesystem::path> shaderPath;
	shaderPath.push_back( "Shaders/vert.vert" );
	shaderPath.push_back( "Shaders/frag.frag" );
	if (!pipeline->init( shaderPath, swapchain->getExtent(), renderPass->getRenderPass() )) return false;
	return true;
}

bool Context::initFrameBuffer()
{
	framebuffers = new v3d::vulkan::Framebuffers( *swapchain, renderPass->getRenderPass() );
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
		imageAvailableSemaphores.push_back( logicalDevice->getVKLogicalDevice().createSemaphore( vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() ) ) );
		renderFinishedSemaphores.push_back( logicalDevice->getVKLogicalDevice().createSemaphore( vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() ) ) );
	}

	return true;
}

bool Context::initFences()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		frameFences.push_back( logicalDevice->getVKLogicalDevice().createFence( vk::FenceCreateInfo( vk::FenceCreateFlagBits::eSignaled ) ) );
	}

	return true;
}

bool Context::initCommandBuffer()
{
	//const std::vector<vk::Framebuffer>& fbs = framebuffers->getFramebuffers();
	const std::size_t fbSize = framebuffers->size();

	//vk::CommandBufferAllocateInfo allocInfo
	//(
	//	commandPool->get(),
	//	vk::CommandBufferLevel::ePrimary,
	//	static_cast<uint32_t>(fbSize)
	//);

	//const std::vector<vk::CommandBuffer> cbs = logicalDevice->getVKLogicalDevice().allocateCommandBuffers( allocInfo );

	for (std::size_t i = 0; i < fbSize; i++)
	{
		auto newCB = new v3d::vulkan::CommandBuffer( vk::CommandBufferLevel::ePrimary );
		commandBuffers.push_back( newCB );
		newCB->begin( vk::CommandBufferUsageFlagBits::eSimultaneousUse );

		const vk::CommandBuffer& cb = newCB->getVKCommandBuffer();

		vk::ClearValue clearValue( vk::ClearColorValue( std::array<float, 4>( { 0.2f, 0.2f, 0.2f, 0.2f } ) ) );

		vk::RenderPassBeginInfo renderPassInfo
		(
			renderPass->getRenderPass(),
			(*framebuffers)[i],
			vk::Rect2D
			(
				vk::Offset2D(),
				swapchain->getExtent()
			),
			1,
			&clearValue
		);

		cb.beginRenderPass( renderPassInfo, vk::SubpassContents::eInline );
		cb.bindPipeline( vk::PipelineBindPoint::eGraphics, pipeline->getVKPipeline() );
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

	descriptorLayout = logicalDevice->getVKLogicalDevice().createDescriptorSetLayout( layoutInfo );

	return true;
}
*/

bool Context::initDescriptorPool()
{
	const uint32_t count = uint32_t( framebuffers->size() );

	vk::DescriptorPoolSize uboPoolSize
	(
		vk::DescriptorType::eUniformBuffer,
		count
	);

	vk::DescriptorPoolSize lenaSamplerPoolSize
	(
		vk::DescriptorType::eCombinedImageSampler,
		count
	);

	const uint32_t size = 2;
	vk::DescriptorPoolSize poolSizes[size] = { uboPoolSize, lenaSamplerPoolSize };

	vk::DescriptorPoolCreateInfo poolInfo
	(
		vk::DescriptorPoolCreateFlags(),
		count,
		size,
		poolSizes
	);

	descriptorPool = logicalDevice->getVKLogicalDevice().createDescriptorPool( poolInfo );

	return true;
}

bool Context::initDescriptorSet()
{
	const std::size_t count = framebuffers->size();

	std::vector<vk::DescriptorSetLayout> layouts( count, pipeline->getDescriptorSetLayout() );
	vk::DescriptorSetAllocateInfo allocInfo
	(
		descriptorPool,
		static_cast<uint32_t>(count),
		layouts.data()
	);

	descriptorSets = logicalDevice->getVKLogicalDevice().allocateDescriptorSets( allocInfo );

	for (std::size_t i = 0; i < count; i++)
	{
		vk::DescriptorBufferInfo mvpUBOInfo
		(
			mvpUBOs[i]->getBuffer(),
			vk::DeviceSize( 0 ),
			vk::DeviceSize( sizeof( glm::mat4 ) * 3 )
		);

		v3d::vulkan::Image* texImg = lena->getImage();
		vk::DescriptorImageInfo lenaImageInfo
		(
			texImg->getSampler(),
			texImg->getImageview(),
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

		const uint32_t descriptorWriteSize = 2;
		const vk::WriteDescriptorSet descriptorWrites[descriptorWriteSize] = { mvpUBODescriptorWrite, lenaSamplerDescriptorWrite };

		logicalDevice->getVKLogicalDevice().updateDescriptorSets( descriptorWriteSize, descriptorWrites, 0, nullptr );
	}

	return true;
}







bool Context::recreateSwapChain()
{
	logicalDevice->getVKLogicalDevice().waitIdle();

	releaseSwapChain();

	if (!initSwapChain()) return false;
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
	return v3d::vulkan::CommandBuffer( logicalDevice->getVKLogicalDevice().allocateCommandBuffers( allocInfo ).front() );
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

	//logicalDevice->getVKLogicalDevice().freeCommandBuffers( commandPool->get(), oneTimeCB.get() );
}

void Context::createLenaBuffer()
{
	lenaBuffer.vertexBuffer = new v3d::vulkan::Buffer( lenaBuffer.vertexData.getDataSize(), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal );
	lenaBuffer.indexBuffer = new v3d::vulkan::Buffer( lenaBuffer.indexData.getDataSize(), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal );

	const vk::Device& ld = logicalDevice->getVKLogicalDevice();

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
	const std::size_t size = framebuffers->size();
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

	static float time = 1.0f / 60.0f;
	const float angle = 0.0f;
	static float curAngle = 0.0f;
	curAngle += (30.0f * time);
	if (curAngle >= 360.0f) curAngle -= 360.0f;

	curMVP.model = screenSpaceMatrix * glm::rotate( glm::mat4( 1 ), glm::radians( curAngle ), glm::vec3( 0, 0, 1 ) );

	static struct Camera
	{
		glm::vec3 pos;
		glm::vec3 rot;
	} cam;

	cam.pos = glm::vec3( 0, 2, -10 );
	curMVP.view = glm::translate( glm::mat4( 1 ), glm::vec3( cam.pos.x, cam.pos.y, cam.pos.z ) );

	//const float fovy = 70.0f;
	const auto& extent = swapchain->getExtent();
	const float aspect = static_cast<float>(extent.width) / static_cast<float>(extent.height);
	const float nears = 0.1f;
	const float fars = 1000.0f;
	curMVP.projection = glm::perspective( glm::radians( fovy ), aspect, nears, fars );

	mvpUBOs[imageIndex]->update( &curMVP );
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
	logicalDevice->getVKLogicalDevice().waitForFences( 1, &frameFences[current_frame], true, std::numeric_limits<uint64_t>::max() );

	const vk::ResultValue<uint32_t> result = logicalDevice->getVKLogicalDevice().acquireNextImageKHR( swapchain->getSwapchainKHR(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[current_frame], nullptr );
	if (result.result == vk::Result::eErrorOutOfDateKHR)
	{
		recreateSwapChain();
		return;
	}
	else if (result.result != vk::Result::eSuccess && result.result != vk::Result::eSuboptimalKHR)
	{
		throw std::runtime_error( "failed to acquire swap chain image!" );
	}
	assert( result.value < framebuffers->size() );

	updateMVPUBO( result.value );

	vk::Semaphore waitSemaphores[] = { imageAvailableSemaphores[current_frame] };
	vk::Semaphore signalSemaphores[] = { renderFinishedSemaphores[current_frame] };
	vk::PipelineStageFlags waitFlags[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
	vk::CommandBuffer commandBuffers[] = { this->commandBuffers.at( result.value )->getVKCommandBuffer() };
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

	logicalDevice->getVKLogicalDevice().resetFences( frameFences[current_frame] );

	logicalDevice->getGraphicsQueue().submit( submitInfo, frameFences[current_frame] );

	vk::SwapchainKHR swapChains[] = { swapchain->getSwapchainKHR() };
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
	logicalDevice->getVKLogicalDevice().waitIdle();
}




v3d::vulkan::Instance* Context::getInstance() const
{
	return instance;
}

const vk::SurfaceKHR& Context::getSurface() const
{
	return surface;
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

	//SAFE_DELETE( lena.imageSource );

	//logicalDevice->getVKLogicalDevice().destroySampler( lena.sampler );
	//logicalDevice->getVKLogicalDevice().destroyImageView( lena.imageView );
	//logicalDevice->getVKLogicalDevice().destroyImage( lena.image );
	//logicalDevice->getVKLogicalDevice().freeMemory( lena.deviceMemory );

	SAFE_DELETE( lenaBuffer.vertexBuffer );
	SAFE_DELETE( lenaBuffer.indexBuffer );
	SAFE_DELETE( lena );

	for (auto& f : frameFences) { logicalDevice->getVKLogicalDevice().destroyFence( f ); }
	frameFences.clear();

	for (auto& s : imageAvailableSemaphores) { logicalDevice->getVKLogicalDevice().destroySemaphore( s ); }
	imageAvailableSemaphores.clear();
	for (auto& s : renderFinishedSemaphores) { logicalDevice->getVKLogicalDevice().destroySemaphore( s ); }
	renderFinishedSemaphores.clear();

	releaseSwapChain();

	SAFE_DELETE( physicalDevice );
	SAFE_DELETE( logicalDevice );

	instance->getVKInstance().destroySurfaceKHR( surface );
	SAFE_DELETE( instance );

	logger.info( "Releasing Context finished" );
}

void Context::releaseSwapChain()
{
	const vk::Device& ld = logicalDevice->getVKLogicalDevice();

	for (auto mvpUBO : mvpUBOs) { SAFE_DELETE( mvpUBO ); }
	mvpUBOs.clear();

	//ld.destroyDescriptorSetLayout( descriptorLayout );
	ld.destroyDescriptorPool( descriptorPool );

	for (auto& cb : commandBuffers) SAFE_DELETE( cb );
	commandBuffers.clear();

	SAFE_DELETE( commandPool );
	SAFE_DELETE( framebuffers );
	SAFE_DELETE( pipeline );
	SAFE_DELETE( renderPass );
	SAFE_DELETE( swapchain );
}

VK_NS_END
V3D_NS_END