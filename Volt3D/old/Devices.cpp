/**
*	@file Devices.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Devices.h"

#include "Utils.h"
#include "Utils/Logger.h"
#include "Config/BuildConfig.h"

v3d::vulkan::Devices::Devices()
	: physicalDevice()
	, logicalDevice()
	, properties()
	, features()
	, memoryProperties()
	, QFI( { 0,0,0,0 } )
{}

bool v3d::vulkan::Devices::isSuitablePhysicalDevice( const vk::PhysicalDevice& physicalDevice )
{
	auto& logger = v3d::Logger::getInstance();

	static auto bitToStr = []( uint64_t size ) -> std::string
	{
		const std::vector<std::string> SUFFIXES{ { "B", "KB", "MB", "GB", "TB", "PB" } };
		size_t suffixIndex = 0;
		while (suffixIndex < SUFFIXES.size() - 1 && size > 1024) {
			size >>= 10;
			++suffixIndex;
		}

		std::stringstream buffer;
		buffer << size << " " << SUFFIXES[suffixIndex];
		return buffer.str();
	};

	vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();

	if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
	{
		logger.info( "Found discrete GPU" );

		auto physicalDeviceMemoryProperties = physicalDevice.getMemoryProperties();

		logger.trace( "GPU information ---" );
		logger.trace( "Driver Version: " + std::to_string( properties.driverVersion ) );
		logger.trace( "Device Name: " + std::string( properties.deviceName ) );
		logger.trace( "Device Type: VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU" );
		const int major = (properties.apiVersion >> 22) & 0x3FF;
		const int minor = (properties.apiVersion >> 12) & 0x3FF;
		const int version = (properties.apiVersion & 0xFFF);
		logger.trace( "API Version: " + std::to_string( major ) + "." + std::to_string( minor ) + "." + std::to_string( version ) );
		logger.trace( "Memory heaps: " + std::to_string( physicalDeviceMemoryProperties.memoryHeapCount ) );
		for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryHeapCount; ++i)
		{
			const auto& heap = physicalDeviceMemoryProperties.memoryHeaps[i];
			logger.trace( "\tHeap " + std::to_string( i ) + " flags: " + vk::to_string( heap.flags ) + ", size: " + std::to_string( heap.size ) + " (" + bitToStr( heap.size ) + ")" );
		}
		logger.trace( "Memory type: " + std::to_string( physicalDeviceMemoryProperties.memoryTypeCount ) );
		for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; ++i)
		{
			const auto& type = physicalDeviceMemoryProperties.memoryTypes[i];
			logger.trace( "\tType " + std::to_string( i ) + " flags: " + vk::to_string( type.propertyFlags ) + ", heap: " + std::to_string( type.heapIndex ) );
		}
		logger.trace( "-------------------" );

		return true;
	}

	logger.critical( "Failed to find discrete GPU" );

	return false;
}

bool v3d::vulkan::Devices::initPhysicalDevice( const std::vector<vk::PhysicalDevice>& physicalDevices )
{
	for (const vk::PhysicalDevice& curPD : physicalDevices)
	{
		if (isSuitablePhysicalDevice( curPD ))
		{
			physicalDevice = curPD;

			properties = physicalDevice.getProperties();

#ifdef BUILD_DEBUG
			auto& logger = v3d::Logger::getInstance();
			logger.trace( "maxImageDimension1D: " + std::to_string( properties.limits.maxImageDimension1D ) );
			logger.trace( "maxImageDimension2D: " + std::to_string( properties.limits.maxImageDimension2D ) );
			logger.trace( "maxImageDimension3D: " + std::to_string( properties.limits.maxImageDimension3D ) );
			logger.trace( "maxImageDimensionCube: " + std::to_string( properties.limits.maxImageDimensionCube ) );
			logger.trace( "maxImageArrayLayers: " + std::to_string( properties.limits.maxImageArrayLayers ) );
			logger.trace( "maxTexelBufferElements: " + std::to_string( properties.limits.maxTexelBufferElements ) );
			logger.trace( "maxUniformBufferRange: " + std::to_string( properties.limits.maxUniformBufferRange ) );
			logger.trace( "maxStorageBufferRange: " + std::to_string( properties.limits.maxStorageBufferRange ) );
			logger.trace( "maxPushConstantsSize: " + std::to_string( properties.limits.maxPushConstantsSize ) );
			logger.trace( "maxMemoryAllocationCount: " + std::to_string( properties.limits.maxMemoryAllocationCount ) );
			logger.trace( "maxSamplerAllocationCount: " + std::to_string( properties.limits.maxSamplerAllocationCount ) );
			logger.trace( "bufferImageGranularity: " + std::to_string( properties.limits.bufferImageGranularity ) );
			logger.trace( "sparseAddressSpaceSize: " + std::to_string( properties.limits.sparseAddressSpaceSize ) );
			logger.trace( "maxBoundDescriptorSets: " + std::to_string( properties.limits.maxBoundDescriptorSets ) );
			logger.trace( "maxPerStageDescriptorSamplers: " + std::to_string( properties.limits.maxPerStageDescriptorSamplers ) );
			logger.trace( "maxPerStageDescriptorUniformBuffers: " + std::to_string( properties.limits.maxPerStageDescriptorUniformBuffers ) );
			logger.trace( "maxPerStageDescriptorStorageBuffers: " + std::to_string( properties.limits.maxPerStageDescriptorStorageBuffers ) );
			logger.trace( "maxPerStageDescriptorSampledImages: " + std::to_string( properties.limits.maxPerStageDescriptorSampledImages ) );
			logger.trace( "maxPerStageDescriptorStorageImages: " + std::to_string( properties.limits.maxPerStageDescriptorStorageImages ) );
			logger.trace( "maxPerStageDescriptorInputAttachments: " + std::to_string( properties.limits.maxPerStageDescriptorInputAttachments ) );
			logger.trace( "maxPerStageResources: " + std::to_string( properties.limits.maxPerStageResources ) );
			logger.trace( "maxDescriptorSetSamplers: " + std::to_string( properties.limits.maxDescriptorSetSamplers ) );
			logger.trace( "maxDescriptorSetUniformBuffers: " + std::to_string( properties.limits.maxDescriptorSetUniformBuffers ) );
			logger.trace( "maxDescriptorSetUniformBuffersDynamic: " + std::to_string( properties.limits.maxDescriptorSetUniformBuffersDynamic ) );
			logger.trace( "maxDescriptorSetStorageBuffers: " + std::to_string( properties.limits.maxDescriptorSetStorageBuffers ) );
			logger.trace( "maxDescriptorSetStorageBuffersDynamic: " + std::to_string( properties.limits.maxDescriptorSetStorageBuffersDynamic ) );
			logger.trace( "maxDescriptorSetSampledImages: " + std::to_string( properties.limits.maxDescriptorSetSampledImages ) );
			logger.trace( "maxDescriptorSetStorageImages: " + std::to_string( properties.limits.maxDescriptorSetStorageImages ) );
			logger.trace( "maxDescriptorSetInputAttachments: " + std::to_string( properties.limits.maxDescriptorSetInputAttachments ) );
			logger.trace( "maxVertexInputAttributes: " + std::to_string( properties.limits.maxVertexInputAttributes ) );
			logger.trace( "maxVertexInputBindings: " + std::to_string( properties.limits.maxVertexInputBindings ) );
			logger.trace( "maxVertexInputAttributeOffset: " + std::to_string( properties.limits.maxVertexInputAttributeOffset ) );
			logger.trace( "maxVertexInputBindingStride: " + std::to_string( properties.limits.maxVertexInputBindingStride ) );
			logger.trace( "maxVertexOutputComponents: " + std::to_string( properties.limits.maxVertexOutputComponents ) );
			logger.trace( "maxTessellationGenerationLevel: " + std::to_string( properties.limits.maxTessellationGenerationLevel ) );
			logger.trace( "maxTessellationPatchSize: " + std::to_string( properties.limits.maxTessellationPatchSize ) );
			logger.trace( "maxTessellationControlPerVertexInputComponents: " + std::to_string( properties.limits.maxTessellationControlPerVertexInputComponents ) );
			logger.trace( "maxTessellationControlPerVertexOutputComponents: " + std::to_string( properties.limits.maxTessellationControlPerVertexOutputComponents ) );
			logger.trace( "maxTessellationControlPerPatchOutputComponents: " + std::to_string( properties.limits.maxTessellationControlPerPatchOutputComponents ) );
			logger.trace( "maxTessellationControlTotalOutputComponents: " + std::to_string( properties.limits.maxTessellationControlTotalOutputComponents ) );
			logger.trace( "maxTessellationEvaluationInputComponents: " + std::to_string( properties.limits.maxTessellationEvaluationInputComponents ) );
			logger.trace( "maxTessellationEvaluationOutputComponents: " + std::to_string( properties.limits.maxTessellationEvaluationOutputComponents ) );
			logger.trace( "maxGeometryShaderInvocations: " + std::to_string( properties.limits.maxGeometryShaderInvocations ) );
			logger.trace( "maxGeometryInputComponents: " + std::to_string( properties.limits.maxGeometryInputComponents ) );
			logger.trace( "maxGeometryOutputComponents: " + std::to_string( properties.limits.maxGeometryOutputComponents ) );
			logger.trace( "maxGeometryOutputVertices: " + std::to_string( properties.limits.maxGeometryOutputVertices ) );
			logger.trace( "maxGeometryTotalOutputComponents: " + std::to_string( properties.limits.maxGeometryTotalOutputComponents ) );
			logger.trace( "maxFragmentInputComponents: " + std::to_string( properties.limits.maxFragmentInputComponents ) );
			logger.trace( "maxFragmentOutputAttachments: " + std::to_string( properties.limits.maxFragmentOutputAttachments ) );
			logger.trace( "maxFragmentDualSrcAttachments: " + std::to_string( properties.limits.maxFragmentDualSrcAttachments ) );
			logger.trace( "maxFragmentCombinedOutputResources: " + std::to_string( properties.limits.maxFragmentCombinedOutputResources ) );
			logger.trace( "maxComputeSharedMemorySize: " + std::to_string( properties.limits.maxComputeSharedMemorySize ) );
			logger.trace( "maxComputeWorkGroupCount[0]: " + std::to_string( properties.limits.maxComputeWorkGroupCount[0] ) );
			logger.trace( "maxComputeWorkGroupCount[1]: " + std::to_string( properties.limits.maxComputeWorkGroupCount[1] ) );
			logger.trace( "maxComputeWorkGroupCount[2]: " + std::to_string( properties.limits.maxComputeWorkGroupCount[2] ) );
			logger.trace( "maxComputeWorkGroupInvocations: " + std::to_string( properties.limits.maxComputeWorkGroupInvocations ) );
			logger.trace( "maxComputeWorkGroupSize[0]: " + std::to_string( properties.limits.maxComputeWorkGroupSize[0] ) );
			logger.trace( "maxComputeWorkGroupSize[1]: " + std::to_string( properties.limits.maxComputeWorkGroupSize[1] ) );
			logger.trace( "maxComputeWorkGroupSize[2]: " + std::to_string( properties.limits.maxComputeWorkGroupSize[2] ) );
			logger.trace( "subPixelPrecisionBits: " + std::to_string( properties.limits.subPixelPrecisionBits ) );
			logger.trace( "subTexelPrecisionBits: " + std::to_string( properties.limits.subTexelPrecisionBits ) );
			logger.trace( "mipmapPrecisionBits: " + std::to_string( properties.limits.mipmapPrecisionBits ) );
			logger.trace( "maxDrawIndexedIndexValue: " + std::to_string( properties.limits.maxDrawIndexedIndexValue ) );
			logger.trace( "maxDrawIndirectCount: " + std::to_string( properties.limits.maxDrawIndirectCount ) );
			logger.trace( "maxSamplerLodBias: " + std::to_string( properties.limits.maxSamplerLodBias ) );
			logger.trace( "maxSamplerAnisotropy: " + std::to_string( properties.limits.maxSamplerAnisotropy ) );
			logger.trace( "maxViewports: " + std::to_string( properties.limits.maxViewports ) );
			logger.trace( "maxViewportDimensions[0]: " + std::to_string( properties.limits.maxViewportDimensions[0] ) );
			logger.trace( "maxViewportDimensions[1]: " + std::to_string( properties.limits.maxViewportDimensions[1] ) );
			logger.trace( "viewportBoundsRange[0]: " + std::to_string( properties.limits.viewportBoundsRange[0] ) );
			logger.trace( "viewportBoundsRange[1]: " + std::to_string( properties.limits.viewportBoundsRange[1] ) );
			logger.trace( "viewportSubPixelBits: " + std::to_string( properties.limits.viewportSubPixelBits ) );
			logger.trace( "minMemoryMapAlignment: " + std::to_string( properties.limits.minMemoryMapAlignment ) );
			logger.trace( "minTexelBufferOffsetAlignment: " + std::to_string( properties.limits.minTexelBufferOffsetAlignment ) );
			logger.trace( "minUniformBufferOffsetAlignment: " + std::to_string( properties.limits.minUniformBufferOffsetAlignment ) );
			logger.trace( "minStorageBufferOffsetAlignment: " + std::to_string( properties.limits.minStorageBufferOffsetAlignment ) );
			logger.trace( "minTexelOffset: " + std::to_string( properties.limits.minTexelOffset ) );
			logger.trace( "maxTexelOffset: " + std::to_string( properties.limits.maxTexelOffset ) );
			logger.trace( "minTexelGatherOffset: " + std::to_string( properties.limits.minTexelGatherOffset ) );
			logger.trace( "maxTexelGatherOffset: " + std::to_string( properties.limits.maxTexelGatherOffset ) );
			logger.trace( "minInterpolationOffset: " + std::to_string( properties.limits.minInterpolationOffset ) );
			logger.trace( "maxInterpolationOffset: " + std::to_string( properties.limits.maxInterpolationOffset ) );
			logger.trace( "subPixelInterpolationOffsetBits: " + std::to_string( properties.limits.subPixelInterpolationOffsetBits ) );
			logger.trace( "maxFramebufferWidth: " + std::to_string( properties.limits.maxFramebufferWidth ) );
			logger.trace( "maxFramebufferHeight: " + std::to_string( properties.limits.maxFramebufferHeight ) );
			logger.trace( "maxFramebufferLayers: " + std::to_string( properties.limits.maxFramebufferLayers ) );
			logger.trace( "framebufferColorSampleCounts: " + to_string( properties.limits.framebufferColorSampleCounts ) );
			logger.trace( "framebufferDepthSampleCounts: " + to_string( properties.limits.framebufferDepthSampleCounts ) );
			logger.trace( "framebufferStencilSampleCounts: " + to_string( properties.limits.framebufferStencilSampleCounts ) );
			logger.trace( "framebufferNoAttachmentsSampleCounts: " + to_string( properties.limits.framebufferNoAttachmentsSampleCounts ) );
			logger.trace( "maxColorAttachments: " + std::to_string( properties.limits.maxColorAttachments ) );
			logger.trace( "sampledImageColorSampleCounts: " + to_string( properties.limits.sampledImageColorSampleCounts ) );
			logger.trace( "sampledImageIntegerSampleCounts: " + to_string( properties.limits.sampledImageIntegerSampleCounts ) );
			logger.trace( "sampledImageDepthSampleCounts: " + to_string( properties.limits.sampledImageDepthSampleCounts ) );
			logger.trace( "sampledImageStencilSampleCounts: " + to_string( properties.limits.sampledImageStencilSampleCounts ) );
			logger.trace( "storageImageSampleCounts: " + to_string( properties.limits.storageImageSampleCounts ) );
			logger.trace( "maxSampleMaskWords: " + std::to_string( properties.limits.maxSampleMaskWords ) );
			logger.trace( "timestampComputeAndGraphics: " + std::to_string( properties.limits.timestampComputeAndGraphics ) );
			logger.trace( "timestampPeriod: " + std::to_string( properties.limits.timestampPeriod ) );
			logger.trace( "maxClipDistances: " + std::to_string( properties.limits.maxClipDistances ) );
			logger.trace( "maxCullDistances: " + std::to_string( properties.limits.maxCullDistances ) );
			logger.trace( "maxCombinedClipAndCullDistances: " + std::to_string( properties.limits.maxCombinedClipAndCullDistances ) );
			logger.trace( "discreteQueuePriorities: " + std::to_string( properties.limits.discreteQueuePriorities ) );
			logger.trace( "pointSizeRange[0]: " + std::to_string( properties.limits.pointSizeRange[0] ) );
			logger.trace( "pointSizeRange[1]: " + std::to_string( properties.limits.pointSizeRange[1] ) );
			logger.trace( "lineWidthRange[0]: " + std::to_string( properties.limits.lineWidthRange[0] ) );
			logger.trace( "lineWidthRange[1]: " + std::to_string( properties.limits.lineWidthRange[1] ) );
			logger.trace( "pointSizeGranularity: " + std::to_string( properties.limits.pointSizeGranularity ) );
			logger.trace( "lineWidthGranularity: " + std::to_string( properties.limits.lineWidthGranularity ) );
			logger.trace( "strictLines: " + std::to_string( properties.limits.strictLines ) );
			logger.trace( "standardSampleLocations: " + std::to_string( properties.limits.standardSampleLocations ) );
			logger.trace( "optimalBufferCopyOffsetAlignment: " + std::to_string( properties.limits.optimalBufferCopyOffsetAlignment ) );
			logger.trace( "optimalBufferCopyRowPitchAlignment: " + std::to_string( properties.limits.optimalBufferCopyRowPitchAlignment ) );
			logger.trace( "nonCoherentAtomSize: " + std::to_string( properties.limits.nonCoherentAtomSize ) );
#endif

			features = physicalDevice.getFeatures();
			memoryProperties = physicalDevice.getMemoryProperties();
			queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

			return true;
		}
	}

	return false;
}

bool v3d::vulkan::Devices::initLogicalDevice( const vk::SurfaceKHR& surface )
{
	if (!initQueueFamilyIndices( surface )) return false;

	const float queuePriority = 1.0f;
	vk::DeviceQueueCreateInfo deviceQueueCreateInfo = vk::DeviceQueueCreateInfo
	(
		{},
		QFI.graphics,
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

	return true;
}

bool v3d::vulkan::Devices::initQueueFamilyIndices( const vk::SurfaceKHR& surface )
{
	uint32_t i = 0;
	for (auto& queueFamilyProperty : queueFamilyProperties)
	{
		if (queueFamilyProperty.queueCount <= 0) continue;

		vk::Bool32 supportSurface = physicalDevice.getSurfaceSupportKHR( i, surface );
		if (!supportSurface) continue;

		if (queueFamilyProperty.queueFlags & (vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute | vk::QueueFlagBits::eTransfer))
		{
			QFI = { i,i,i,i };
			return true;
		}

		i++;
	}

	return false;
}

vk::SurfaceCapabilitiesKHR v3d::vulkan::Devices::getSurfaceCapabilitiesKHR(const vk::SurfaceKHR& surface) const
{
	return physicalDevice.getSurfaceCapabilitiesKHR(surface);
}

vk::Bool32 v3d::vulkan::Devices::getSurfaceSupportKHR(const uint32_t index, const vk::SurfaceKHR& surface) const
{
	return physicalDevice.getSurfaceSupportKHR(index, surface);
}

std::vector<vk::SurfaceFormatKHR> v3d::vulkan::Devices::getSurfaceFormatsKHR(const vk::SurfaceKHR& surface) const
{
	return physicalDevice.getSurfaceFormatsKHR(surface);
}

std::vector<vk::PresentModeKHR> v3d::vulkan::Devices::getSurfacePresentModesKHR(const vk::SurfaceKHR& surface) const
{
	return physicalDevice.getSurfacePresentModesKHR(surface);
}

vk::PhysicalDeviceProperties v3d::vulkan::Devices::getProperties() const
{
	return properties;
}

vk::PhysicalDeviceFeatures v3d::vulkan::Devices::getFeatures() const
{
	return features;
}

vk::PhysicalDeviceMemoryProperties v3d::vulkan::Devices::getMemoryProperties() const
{
	return memoryProperties;
}

std::vector<vk::ExtensionProperties> v3d::vulkan::Devices::EnumerateDeviceExtensionProperties() const
{
	return physicalDevice.enumerateDeviceExtensionProperties();
}

std::vector<vk::LayerProperties> v3d::vulkan::Devices::enumerateDeviceLayerProperties() const
{
	return physicalDevice.enumerateDeviceLayerProperties();
}

vk::PhysicalDeviceLimits v3d::vulkan::Devices::getLimits() const
{
	return properties.limits;
}

uint32_t v3d::vulkan::Devices::getGraphicsQueueFamilyIndex() const
{
	return QFI.graphics;
}

uint32_t v3d::vulkan::Devices::getComputeQueueFamilyIndex() const
{
	return QFI.compute;
}

uint32_t v3d::vulkan::Devices::getTransferQueueFamilyIndex() const
{
	return QFI.transfer;
}

uint32_t v3d::vulkan::Devices::getPresentQueueFamilyIndex() const
{
	return QFI.present;
}

uint32_t v3d::vulkan::Devices::getMemoryTypeIndex(const uint32_t memoryTypeBits, const vk::MemoryPropertyFlags memoryPropertyFlags) const
{
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((memoryTypeBits & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)
		{
			return i;
		}
	}
	
	throw std::runtime_error("failed to find suitable memory type!");
}

vk::Buffer v3d::vulkan::Devices::createBuffer( const uint64_t size, const vk::BufferUsageFlags usageFlags ) const
{
	vk::BufferCreateInfo createInfo( vk::BufferCreateFlags(), size, usageFlags );
	return logicalDevice.createBuffer( createInfo );
}

vk::DeviceMemory v3d::vulkan::Devices::createDeviceMemory( const vk::Buffer& buffer, const vk::MemoryPropertyFlags memoryPropertyFlags ) const
{
	const vk::MemoryRequirements memRequirements = logicalDevice.getBufferMemoryRequirements( buffer );
	const vk::MemoryAllocateInfo allocInfo
	(
		memRequirements.size,
		getMemoryTypeIndex( memRequirements.memoryTypeBits, memoryPropertyFlags )
	);

	vk::DeviceMemory deviceMemory = logicalDevice.allocateMemory( allocInfo );
	logicalDevice.bindBufferMemory( buffer, deviceMemory, vk::DeviceSize( 0 ) );
	return deviceMemory;
}