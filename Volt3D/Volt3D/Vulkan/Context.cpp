#include <PreCompiled.h>

#include "Context.h"

#include <glm/glm.hpp>

#include "Core/View.h"
#include "DebugCallback.h"	
#include "Surface.h"
#include "PhysicalDevice.h"
#include "Device.h"
#include "SwapChain.h"
#include "Shader.h"
#include "Utils.h"

v3d::vulkan::Context::Context()
	: instance()
	, enableValidationLayer(false)
	, debugCallback(nullptr)
	, surface(nullptr)
	, physicalDevice(nullptr)
	, device(nullptr)
	, swapChain(nullptr)
{}

v3d::vulkan::Context::~Context()
{
	release();
}

bool v3d::vulkan::Context::init( const View& view, const bool enableValidationLayer )
{
	// Init logger
	auto& logger = Logger::getInstance();

	// Get version
	uint32_t major, minor, patch;
	if( !vulkan::utils::getVersion( major, minor, patch ) ) { logger.critical( "Failed to get Context version." ); return false; }
	logger.trace( "Context version: " + std::to_string( major ) + "." + std::to_string( minor ) + "." + std::to_string( patch ) );

	this->enableValidationLayer = enableValidationLayer;

	if( !initInstance( view ) ) return false;
	if( !initSurface( view ) ) return false;
	if( !initPhysicalDevice() ) return false;
	if( !initLogicalDevice() ) return false;
	if( !initSwapChain() ) return false;

	return true;
}

bool v3d::vulkan::Context::initInstance( const View & view )
{
	auto& logger = Logger::getInstance();

	vk::ApplicationInfo appInfo( "Learn Context", VK_MAKE_VERSION( 1, 0, 0 ), "Context", VK_MAKE_VERSION( 1, 0, 0 ), VK_API_VERSION_1_1 );

	std::vector<vk::ExtensionProperties> extensions = getExtensions();
#if _DEBUG
	logger.logExtensions( extensions );
#endif

	std::vector<vk::LayerProperties> layers = getLayers();
#if _DEBUG
	logger.logLayers( layers );
#endif
	std::vector<const char*> requiredExtensions;
	view.getGLFWVKExtensions( requiredExtensions );
	if( _DEBUG ) 
	{
		requiredExtensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
		requiredExtensions.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
	}

	if( !vulkan::utils::checkExtensionProperties( extensions, requiredExtensions ) ) return false;

	std::vector<const char*> requiredLayers;
	if( _DEBUG ) requiredLayers.push_back( "VK_LAYER_KHRONOS_validation" );

	if( !vulkan::utils::checkLayerProperties( layers, requiredLayers ) ) return false;

	vk::InstanceCreateInfo createInfo( {}, &appInfo, uint32_t(requiredLayers.size()), requiredLayers.data(), uint32_t(requiredExtensions.size()), requiredExtensions.data() );

	instance = std::move( vk::createInstanceUnique( createInfo ) );

	logger.trace( "Created Context instance" );

	if( _DEBUG )
	{
		if( !initDebugCallback() ) return false;
	}

	return true;
}

bool v3d::vulkan::Context::initDebugCallback()
{
	debugCallback = new (std::nothrow) DebugCallback();
	if( debugCallback == nullptr ) { Logger::getInstance().critical( "std::bad_alloc" ); return false; }
	if( !debugCallback->init( instance ) ) return false;
	return true;
}

bool v3d::vulkan::Context::initSurface( const View& view )
{
	VkSurfaceKHR cVkSurfaceKHR;
	if( !view.createWindowSurface( instance, cVkSurfaceKHR ) ) return false;

	surface = new(std::nothrow) Surface( vk::UniqueSurfaceKHR( cVkSurfaceKHR, instance.get() ) );
	return (surface != nullptr);
}

bool v3d::vulkan::Context::initPhysicalDevice()
{
	physicalDevice = new (std::nothrow) PhysicalDevice();
	if( !physicalDevice ) return false;
	return physicalDevice->init( instance );
}

bool v3d::vulkan::Context::initLogicalDevice()
{
	device = new (std::nothrow) Device();
	if( !device ) return false;
	return device->init( *physicalDevice, *surface );
}

bool v3d::vulkan::Context::initSwapChain()
{
	swapChain = new (std::nothrow) SwapChain();
	if( !swapChain ) return false;
	return swapChain->init( *physicalDevice, *device, *surface );
}

bool v3d::vulkan::Context::initGraphicsPipeline()
{
	//Shader* vertShader = Shader::create( "Shaders/vert.spv", logicalDevice );
	//if( !vertShader ) return false;
	//Shader* fragShader = Shader::create( "Shaders/frag.spv", logicalDevice );
	//if( !fragShader ) return false;
	

	//const auto v = { Shader() };

	//vk::PipelineShaderStageCreateInfo vertCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, &(Shader()), "main" );

	//delete vertShader;
	//delete fragShader;

	return true;
}

void v3d::vulkan::Context::release()
{
	auto& logger = Logger::getInstance();
	logger.info( "Releasing Context..." );
	if( enableValidationLayer && debugCallback ) 
	{ 
		delete debugCallback; 
		debugCallback = nullptr;
	}
	logger.info( "Releasing Context finished" );
}

std::vector<vk::ExtensionProperties> v3d::vulkan::Context::getExtensions() const
{
	return vk::enumerateInstanceExtensionProperties();
}

std::vector<vk::LayerProperties> v3d::vulkan::Context::getLayers() const
{
	return vk::enumerateInstanceLayerProperties();
}