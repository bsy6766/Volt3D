/**
*	@file PhysicalDevice.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "PhysicalDevice.h"

#include "Vulkan/Context.h"
#include "Config/BuildConfig.h"

V3D_NS_BEGIN
VK_NS_BEGIN

PhysicalDevice::PhysicalDevice()
	: physicalDevice( nullptr )
	, properties()
	, features()
	, memoryProperties()
	, sampleCountFlagBits()
{}

PhysicalDevice::~PhysicalDevice() {}

v3d::vulkan::PhysicalDevice* PhysicalDevice::get() { return v3d::vulkan::Context::get()->getPhysicalDevice(); }

const vk::PhysicalDevice& PhysicalDevice::getVKPhysicalDevice() const { return physicalDevice; }

bool PhysicalDevice::init( const std::vector<vk::PhysicalDevice>& physicalDevices )
{
	if (physicalDevices.empty()) return false;

	for (auto& curPD : physicalDevices)
	{
		if (check( curPD ))
		{
			physicalDevice = curPD;

			properties = physicalDevice.getProperties();
			features = physicalDevice.getFeatures();
			initEnabledFeatures();
			memoryProperties = physicalDevice.getMemoryProperties();
			initSampleCount();

#ifdef BUILD_DEBUG
			print( true );
#endif
			return true;
		}
	}

	return false;
}

bool PhysicalDevice::check( const vk::PhysicalDevice& physicalDevice )
{
	auto& logger = v3d::Logger::getInstance();

	vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();

	if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
	{
		// @todo: There can be more option than discrete gpu
		logger.info( "Found discrete GPU" );
		return true;
	}

	logger.critical( "Failed to find discrete GPU" );

	return false;
}

void PhysicalDevice::initSampleCount()
{
	static auto flagToInt = []( vk::SampleCountFlags flag ) -> int
	{
		if (flag & vk::SampleCountFlagBits::e64) return 64;
		else if (flag & vk::SampleCountFlagBits::e32) return 32;
		else if (flag & vk::SampleCountFlagBits::e16) return 16;
		else if (flag & vk::SampleCountFlagBits::e8) return 8;
		else if (flag & vk::SampleCountFlagBits::e4) return 4;
		else if (flag & vk::SampleCountFlagBits::e2) return 2;
		else return 1;
	};

	static auto intToFlags = []( int value ) -> vk::SampleCountFlags
	{
		switch (value)
		{
		case 1: return vk::SampleCountFlagBits::e1;
		case 2: return vk::SampleCountFlagBits::e2;
		case 4: return vk::SampleCountFlagBits::e4;
		case 8: return vk::SampleCountFlagBits::e8;
		case 16: return vk::SampleCountFlagBits::e16;
		case 32: return vk::SampleCountFlagBits::e32;
		case 64: return vk::SampleCountFlagBits::e64;
		default: break;
		}
		return vk::SampleCountFlagBits::e1;
	};

	vk::SampleCountFlags counts = intToFlags( glm::min( flagToInt( properties.limits.framebufferColorSampleCounts ), flagToInt( properties.limits.framebufferDepthSampleCounts ) ) );
	if (counts & vk::SampleCountFlagBits::e64) { sampleCountFlagBits = vk::SampleCountFlagBits::e64; }
	else if (counts & vk::SampleCountFlagBits::e32) { sampleCountFlagBits = vk::SampleCountFlagBits::e32; }
	else if (counts & vk::SampleCountFlagBits::e16) { sampleCountFlagBits = vk::SampleCountFlagBits::e16; }
	else if (counts & vk::SampleCountFlagBits::e8) { sampleCountFlagBits = vk::SampleCountFlagBits::e8; }
	else if (counts & vk::SampleCountFlagBits::e4) { sampleCountFlagBits = vk::SampleCountFlagBits::e4; }
	else if (counts & vk::SampleCountFlagBits::e2) { sampleCountFlagBits = vk::SampleCountFlagBits::e2; }
	else sampleCountFlagBits = vk::SampleCountFlagBits::e1;
}

void PhysicalDevice::initEnabledFeatures()
{
	// @todo Enable features here and make customizable on init...
}

const vk::PhysicalDeviceProperties& PhysicalDevice::getProperties() const
{
	return properties;
}

const vk::PhysicalDeviceFeatures& PhysicalDevice::getFeatures() const
{
	return features;
}

const vk::PhysicalDeviceFeatures& PhysicalDevice::getEnabledFeatures() const
{
	return enabledFeatures;
}

const vk::PhysicalDeviceMemoryProperties& PhysicalDevice::getMemoryProperties() const
{
	return memoryProperties;
}

const vk::SampleCountFlagBits& PhysicalDevice::getSampleCountFlagBits() const
{
	return sampleCountFlagBits;
}

bool PhysicalDevice::isFormatSupported( const vk::Format depthFormat, const vk::ImageTiling tiling, const vk::FormatFeatureFlags& features ) const
{
	const vk::FormatProperties formatProperties = physicalDevice.getFormatProperties( depthFormat );
	
	if (tiling == vk::ImageTiling::eLinear && (formatProperties.linearTilingFeatures & features) == features)
	{
		return true;
	}
	else if (tiling == vk::ImageTiling::eOptimal && (formatProperties.optimalTilingFeatures & features) == features)
	{
		return true;
	}

	return false;
}

uint32_t PhysicalDevice::getMemoryTypeIndex( const uint32_t memoryTypeBits, const vk::MemoryPropertyFlags memoryPropertyFlags ) const
{
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((memoryTypeBits & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)
		{
			return i;
		}
	}

	throw std::runtime_error( "failed to find suitable memory type!" );
}

void PhysicalDevice::print( const bool detail ) const
{
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

	static auto vendorIDToStr = []( uint32_t vendorID ) -> std::string
	{
		switch (vendorID)
		{
		case 0x1002: return "AMD";
		case 0x1010: return "ImgTec";
		case 0x10DE: return "NVIDIA";
		case 0x13B5: return "ARM";
		case 0x5143: return "Qualcomm";
		case 0x8086: return "INTEL";
		default: break;
		}
		return "UnknownVendor";
	};

	auto& logger = v3d::Logger::getInstance();

	logger.trace( "[PhysicalDevice] Info" );

	logger.trace( "GPU information" );
	logger.trace( "Driver Version: " + std::to_string( properties.driverVersion ) );
	logger.trace( "Device Name: " + std::string( properties.deviceName ) );
	logger.trace( "Device Type: " + vk::to_string( properties.deviceType ) );
	logger.trace( "Vendor ID: " + vendorIDToStr( properties.vendorID ) );
	logger.trace( "Device ID: " + std::to_string( properties.deviceID ) );

	const int major = (properties.apiVersion >> 22) & 0x3FF;
	const int minor = (properties.apiVersion >> 12) & 0x3FF;
	const int version = (properties.apiVersion & 0xFFF);
	logger.trace( "API Version: " + std::to_string( major ) + "." + std::to_string( minor ) + "." + std::to_string( version ) );

	if (detail)
	{
		logger.trace( "Limits" );
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

		logger.trace( "Fetures" );
		logger.trace( "robustBufferAccess: " + std::string( features.robustBufferAccess ? "true" : "false" ) );
		logger.trace( "fullDrawIndexUint32: " + std::string( features.fullDrawIndexUint32 ? "true" : "false" ) );
		logger.trace( "imageCubeArray: " + std::string( features.imageCubeArray ? "true" : "false" ) );
		logger.trace( "independentBlend: " + std::string( features.independentBlend ? "true" : "false" ) );
		logger.trace( "geometryShader: " + std::string( features.geometryShader ? "true" : "false" ) );
		logger.trace( "tessellationShader: " + std::string( features.tessellationShader ? "true" : "false" ) );
		logger.trace( "sampleRateShading: " + std::string( features.sampleRateShading ? "true" : "false" ) );
		logger.trace( "dualSrcBlend: " + std::string( features.dualSrcBlend ? "true" : "false" ) );
		logger.trace( "logicOp: " + std::string( features.logicOp ? "true" : "false" ) );
		logger.trace( "multiDrawIndirect: " + std::string( features.multiDrawIndirect ? "true" : "false" ) );
		logger.trace( "drawIndirectFirstInstance: " + std::string( features.drawIndirectFirstInstance ? "true" : "false" ) );
		logger.trace( "depthClamp: " + std::string( features.depthClamp ? "true" : "false" ) );
		logger.trace( "depthBiasClamp: " + std::string( features.depthBiasClamp ? "true" : "false" ) );
		logger.trace( "fillModeNonSolid: " + std::string( features.fillModeNonSolid ? "true" : "false" ) );
		logger.trace( "depthBounds: " + std::string( features.depthBounds ? "true" : "false" ) );
		logger.trace( "wideLines: " + std::string( features.wideLines ? "true" : "false" ) );
		logger.trace( "largePoints: " + std::string( features.largePoints ? "true" : "false" ) );
		logger.trace( "alphaToOne: " + std::string( features.alphaToOne ? "true" : "false" ) );
		logger.trace( "multiViewport: " + std::string( features.multiViewport ? "true" : "false" ) );
		logger.trace( "samplerAnisotropy: " + std::string( features.samplerAnisotropy ? "true" : "false" ) );
		logger.trace( "textureCompressionETC2: " + std::string( features.textureCompressionETC2 ? "true" : "false" ) );
		logger.trace( "textureCompressionASTC_LDR: " + std::string( features.textureCompressionASTC_LDR ? "true" : "false" ) );
		logger.trace( "textureCompressionBC: " + std::string( features.textureCompressionBC ? "true" : "false" ) );
		logger.trace( "occlusionQueryPrecise: " + std::string( features.occlusionQueryPrecise ? "true" : "false" ) );
		logger.trace( "pipelineStatisticsQuery: " + std::string( features.pipelineStatisticsQuery ? "true" : "false" ) );
		logger.trace( "vertexPipelineStoresAndAtomics: " + std::string( features.vertexPipelineStoresAndAtomics ? "true" : "false" ) );
		logger.trace( "fragmentStoresAndAtomics: " + std::string( features.fragmentStoresAndAtomics ? "true" : "false" ) );
		logger.trace( "shaderTessellationAndGeometryPointSize: " + std::string( features.shaderTessellationAndGeometryPointSize ? "true" : "false" ) );
		logger.trace( "shaderImageGatherExtended: " + std::string( features.shaderImageGatherExtended ? "true" : "false" ) );
		logger.trace( "shaderStorageImageExtendedFormats: " + std::string( features.shaderStorageImageExtendedFormats ? "true" : "false" ) );
		logger.trace( "shaderStorageImageMultisample: " + std::string( features.shaderStorageImageMultisample ? "true" : "false" ) );
		logger.trace( "shaderStorageImageReadWithoutFormat: " + std::string( features.shaderStorageImageReadWithoutFormat ? "true" : "false" ) );
		logger.trace( "shaderStorageImageWriteWithoutFormat: " + std::string( features.shaderStorageImageWriteWithoutFormat ? "true" : "false" ) );
		logger.trace( "shaderUniformBufferArrayDynamicIndexing: " + std::string( features.shaderUniformBufferArrayDynamicIndexing ? "true" : "false" ) );
		logger.trace( "shaderSampledImageArrayDynamicIndexing: " + std::string( features.shaderSampledImageArrayDynamicIndexing ? "true" : "false" ) );
		logger.trace( "shaderStorageBufferArrayDynamicIndexing: " + std::string( features.shaderStorageBufferArrayDynamicIndexing ? "true" : "false" ) );
		logger.trace( "shaderStorageImageArrayDynamicIndexing: " + std::string( features.shaderStorageImageArrayDynamicIndexing ? "true" : "false" ) );
		logger.trace( "shaderClipDistance: " + std::string( features.shaderClipDistance ? "true" : "false" ) );
		logger.trace( "shaderCullDistance: " + std::string( features.shaderCullDistance ? "true" : "false" ) );
		logger.trace( "shaderFloat64: " + std::string( features.shaderFloat64 ? "true" : "false" ) );
		logger.trace( "shaderInt64: " + std::string( features.shaderInt64 ? "true" : "false" ) );
		logger.trace( "shaderInt16: " + std::string( features.shaderInt16 ? "true" : "false" ) );
		logger.trace( "shaderResourceResidency: " + std::string( features.shaderResourceResidency ? "true" : "false" ) );
		logger.trace( "shaderResourceMinLod: " + std::string( features.shaderResourceMinLod ? "true" : "false" ) );
		logger.trace( "sparseBinding: " + std::string( features.sparseBinding ? "true" : "false" ) );
		logger.trace( "sparseResidencyBuffer: " + std::string( features.sparseResidencyBuffer ? "true" : "false" ) );
		logger.trace( "sparseResidencyImage2D: " + std::string( features.sparseResidencyImage2D ? "true" : "false" ) );
		logger.trace( "sparseResidencyImage3D: " + std::string( features.sparseResidencyImage3D ? "true" : "false" ) );
		logger.trace( "sparseResidency2Samples: " + std::string( features.sparseResidency2Samples ? "true" : "false" ) );
		logger.trace( "sparseResidency4Samples: " + std::string( features.sparseResidency4Samples ? "true" : "false" ) );
		logger.trace( "sparseResidency8Samples: " + std::string( features.sparseResidency8Samples ? "true" : "false" ) );
		logger.trace( "sparseResidency16Samples: " + std::string( features.sparseResidency16Samples ? "true" : "false" ) );
		logger.trace( "sparseResidencyAliased: " + std::string( features.sparseResidencyAliased ? "true" : "false" ) );
		logger.trace( "variableMultisampleRate: " + std::string( features.variableMultisampleRate ? "true" : "false" ) );
		logger.trace( "inheritedQueries: " + std::string( features.inheritedQueries ? "true" : "false" ) );
	}

	logger.trace( "MemoryProperties" );
	logger.trace( "Memory heaps: " + std::to_string( memoryProperties.memoryHeapCount ) );
	for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i)
	{
		const auto& heap = memoryProperties.memoryHeaps[i];
		logger.trace( "\tHeap " + std::to_string( i ) + " flags: " + vk::to_string( heap.flags ) + ", size: " + std::to_string( heap.size ) + " (" + bitToStr( heap.size ) + ")" );
	}

	logger.trace( "Memory type: " + std::to_string( memoryProperties.memoryTypeCount ) );
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
	{
		const auto& type = memoryProperties.memoryTypes[i];
		logger.trace( "\tType " + std::to_string( i ) + " flags: " + vk::to_string( type.propertyFlags ) + ", heap: " + std::to_string( type.heapIndex ) );
	}

	logger.trace( "SampleCountFlagBits: " + vk::to_string( sampleCountFlagBits ) );
	
	logger.trace( "\n" );
}

VK_NS_END
V3D_NS_END