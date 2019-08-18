/**
*	@file LogicalDevice.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "LogicalDevice.h"

#include <optional>

#include "Vulkan/Context.h"
#include "Vulkan/Utils.h"
#include "Config/BuildConfig.h"

V3D_NS_BEGIN
VK_NS_BEGIN

LogicalDevice::LogicalDevice()
	: logicalDevice( nullptr )
	, graphicsQueueFamilyIndex( 0 )
	, computeQueueFamilyIndex( 0 )
	, transferQueueFamilyIndex( 0 )
	, presentQueueFamilyIndex( 0 )
	, graphicsQueue( nullptr )
	, computeQueue( nullptr )
	, transferQueue( nullptr )
	, presentQueue( nullptr )
{}

LogicalDevice::~LogicalDevice()
{
	logicalDevice.destroy();
}

bool LogicalDevice::init( const vk::SurfaceKHR& surface, const vk::PhysicalDevice& physicalDevice )
{
	const std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

	uint32_t i = 0;
	std::optional<uint32_t> g, c, t, p;

	for (auto& queueFamilyProperty : queueFamilyProperties)
	{
		if (queueFamilyProperty.queueCount <= 0) continue;

		vk::Bool32 supportSurface = physicalDevice.getSurfaceSupportKHR( i, surface );
		if (!supportSurface) continue;

		if (queueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics) g.emplace( i );
		if (g.has_value() && (queueFamilyProperty.queueFlags & vk::QueueFlagBits::eCompute)) c.emplace( i );
		if (g.has_value() && c.has_value() && (queueFamilyProperty.queueFlags & vk::QueueFlagBits::eTransfer)) t.emplace( i );

		if (g.has_value() && c.has_value() && t.has_value())
		{
			p.emplace( i );
			break;
		}

		i++;
	}

	if (!g.has_value() || !c.has_value() || !t.has_value() || !p.has_value()) return false;
	graphicsQueueFamilyIndex = g.value();
	computeQueueFamilyIndex = c.value();
	transferQueueFamilyIndex = t.value();
	presentQueueFamilyIndex = p.value();

	const float queuePriority = 1.0f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo = vk::DeviceQueueCreateInfo
	(
		{},
		graphicsQueueFamilyIndex,
		1,
		&queuePriority
	);

	std::vector<vk::ExtensionProperties> extensions = physicalDevice.enumerateDeviceExtensionProperties();

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

	logicalDevice = physicalDevice.createDevice( createInfo );

#ifdef BUILD_DEBUG
	print();
#endif

	graphicsQueue = logicalDevice.getQueue( graphicsQueueFamilyIndex, 0 );
	computeQueue = logicalDevice.getQueue( computeQueueFamilyIndex, 0 );
	transferQueue = logicalDevice.getQueue( transferQueueFamilyIndex, 0 );
	presentQueue = logicalDevice.getQueue( presentQueueFamilyIndex, 0 );

	return true;
}

v3d::vulkan::LogicalDevice* LogicalDevice::get() { return v3d::vulkan::Context::get()->getLogicalDevice(); }

const vk::Device& LogicalDevice::getVKLogicalDevice() const { return logicalDevice; }

uint32_t LogicalDevice::getGraphicsQueueFamilyIndex() const { return graphicsQueueFamilyIndex; }

uint32_t LogicalDevice::getComputeQueueFamilyIndex() const { return computeQueueFamilyIndex; }

uint32_t LogicalDevice::getTransferQueueFamilyIndex() const { return transferQueueFamilyIndex; }

uint32_t LogicalDevice::getPresentQueueFamilyIndex() const { return presentQueueFamilyIndex; }

const vk::Queue& LogicalDevice::getGraphicsQueue() const { return graphicsQueue; }

const vk::Queue& LogicalDevice::getComputeQueue() const { return computeQueue; }

const vk::Queue& LogicalDevice::getTransferQueue() const { return transferQueue; }

const vk::Queue& LogicalDevice::getPresentQueue() const { return presentQueue; }

void LogicalDevice::print() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.trace( "[LogicalDevice] info" );
	logger.trace( "Graphics queue family index: " + std::to_string( graphicsQueueFamilyIndex ) );
	logger.trace( "Comptue queue family index: " + std::to_string( computeQueueFamilyIndex ) );
	logger.trace( "Transfer queue family index: " + std::to_string( transferQueueFamilyIndex ) );
	logger.trace( "Present queue family index: " + std::to_string( presentQueueFamilyIndex ) );
}

VK_NS_END
V3D_NS_END

