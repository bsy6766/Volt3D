/**
*	@file Image.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Image.h"

#include "Vulkan/Context.h"
#include "Vulkan/Devices/LogicalDevice.h"
#include "Vulkan/Devices/PhysicalDevice.h"
#include "Vulkan/Commands/CommandBuffer.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Image::Image( const vk::Extent3D& extent, const vk::Format& format )
// @todo: Do we really have to store the logical device?
	: logicalDevice( v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice() )
	, image( nullptr )
	, deviceMemory( nullptr )
	, sampler( nullptr )
	, imageView( nullptr )
	, extent( extent )
	, format( format )
	//, filter()
	//, usageFlagBits()
	//, mip_levels( 0 )
	//, layers( 0 )
	//, sampleCount( vk::SampleCountFlagBits::e1 )
	//, samplerAddressMode()
	//, anisotropic( false )
{}

Image::~Image()
{
	logicalDevice.destroySampler( sampler );
	logicalDevice.destroyImageView( imageView );
	logicalDevice.destroyImage( image );
	logicalDevice.freeMemory( deviceMemory );
}

void Image::initImage( const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags )
{
	vk::ImageCreateInfo imageCreateInfo = getImageCreateInfo();
	imageCreateInfo.tiling = tilling;
	imageCreateInfo.usage = usageFlags;
	image = logicalDevice.createImage( imageCreateInfo );
}

void Image::initImageDeviceMemory( const vk::MemoryPropertyFlags memoryPropertyFlags )
{
	vk::MemoryRequirements memRequirements = logicalDevice.getImageMemoryRequirements( image );

	vk::MemoryAllocateInfo allocInfo
	(
		memRequirements.size,
		v3d::vulkan::Context::get()->getPhysicalDevice()->getMemoryTypeIndex( memRequirements.memoryTypeBits, memoryPropertyFlags )
	);

	deviceMemory = logicalDevice.allocateMemory( allocInfo );
	logicalDevice.bindImageMemory( image, deviceMemory, 0 );
}

void Image::initImageView()
{
	vk::ImageViewCreateInfo imageViewCreateInfo
	(
		vk::ImageViewCreateFlags(),
		image,
		vk::ImageViewType::e2D,
		format,
		vk::ComponentMapping( vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA ),
		vk::ImageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 )
	);

	imageView = logicalDevice.createImageView( imageViewCreateInfo, nullptr );
}

void Image::initSampler()
{
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

	sampler = logicalDevice.createSampler( createInfo, nullptr );
}

void Image::transitionLayout( const vk::ImageLayout oldLayout, const vk::ImageLayout newLayout, const vk::PipelineStageFlags srcStage, const vk::PipelineStageFlags dstStage )
{
	auto cb = v3d::vulkan::CommandBuffer( vk::CommandBufferLevel::ePrimary );

	vk::ImageSubresourceRange subresourceRange
	(
		vk::ImageAspectFlagBits::eColor,
		0, 1,	// mipmap
		0, 1	// layer
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

	// Source layouts (old)
	// Source access mask controls actions that have to be finished on the old layout
	// before it will be transitioned to the new layout
	switch (oldLayout)
	{
	case vk::ImageLayout::eUndefined:
		// Image layout is undefined (or does not matter)
		// Only valid as initial layout
		// No flags required, listed only for completeness
		barrier.srcAccessMask = vk::AccessFlags();
		break;

	case vk::ImageLayout::ePreinitialized:
		// Image is preinitialized
		// Only valid as initial layout for linear images, preserves memory contents
		// Make sure host writes have been finished
		barrier.srcAccessMask = vk::AccessFlagBits::eHostWrite;
		break;

	case vk::ImageLayout::eColorAttachmentOptimal:
		// Image is a color attachment
		// Make sure any writes to the color buffer have been finished
		barrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		break;

	case vk::ImageLayout::eDepthStencilAttachmentOptimal:
		// Image is a depth/stencil attachment
		// Make sure any writes to the depth/stencil buffer have been finished
		barrier.srcAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
		break;

	case vk::ImageLayout::eTransferSrcOptimal:
		// Image is a transfer source 
		// Make sure any reads from the image have been finished
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
		break;

	case vk::ImageLayout::eTransferDstOptimal:
		// Image is a transfer destination
		// Make sure any writes to the image have been finished
		barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
		break;

	case vk::ImageLayout::eShaderReadOnlyOptimal:
		// Image is read by a shader
		// Make sure any shader reads from the image have been finished
		barrier.srcAccessMask = vk::AccessFlagBits::eShaderRead;
		break;
	default:
		// Other source layouts aren't handled (yet)
		break;
	}

	// Target layouts (new)
	// Destination access mask controls the dependency for the new image layout
	switch (newLayout)
	{
	case vk::ImageLayout::eTransferDstOptimal:
		// Image will be used as a transfer destination
		// Make sure any writes to the image have been finished
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
		break;

	case vk::ImageLayout::eTransferSrcOptimal:
		// Image will be used as a transfer source
		// Make sure any reads from the image have been finished
		barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
		break;

	case vk::ImageLayout::eColorAttachmentOptimal:
		// Image will be used as a color attachment
		// Make sure any writes to the color buffer have been finished
		barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		break;

	case vk::ImageLayout::eDepthStencilAttachmentOptimal:
		// Image layout will be used as a depth/stencil attachment
		// Make sure any writes to depth/stencil buffer have been finished
		barrier.dstAccessMask = barrier.dstAccessMask | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
		break;

	case vk::ImageLayout::eShaderReadOnlyOptimal:
		// Image will be read in a shader (sampler, input attachment)
		// Make sure any writes to the image have been finished
		if (barrier.srcAccessMask == vk::AccessFlags())
		{
			barrier.srcAccessMask = vk::AccessFlagBits::eHostWrite | vk::AccessFlagBits::eTransferWrite;
		}
		barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
		break;
	default:
		// Other source layouts aren't handled (yet)
		break;
	}

	cb.begin();
	cb.getVKCommandBuffer().pipelineBarrier( srcStage, dstStage, vk::DependencyFlagBits::eByRegion, 0, nullptr, 0, nullptr, 1, &barrier );
	cb.end();

	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cb.getVKCommandBuffer();

	auto& gQueue = v3d::vulkan::LogicalDevice::get()->getGraphicsQueue();
	gQueue.submit( submitInfo, nullptr );
	gQueue.waitIdle();
}

void Image::copyBuffer( const vk::Buffer& stagingBuffer )
{
	auto cb = v3d::vulkan::CommandBuffer( vk::CommandBufferLevel::ePrimary );

	vk::ImageSubresourceLayers imgSubresourceLayer
	(
		vk::ImageAspectFlagBits::eColor,
		0, 0, 1
	);

	vk::BufferImageCopy region
	(
		0, 0, 0,
		imgSubresourceLayer,
		vk::Offset3D(),
		extent
	);

	cb.begin();
	cb.getVKCommandBuffer().copyBufferToImage( stagingBuffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &region );
	cb.end();

	vk::SubmitInfo submitInfo;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &cb.getVKCommandBuffer();

	auto& gQueue = v3d::vulkan::LogicalDevice::get()->getGraphicsQueue();
	gQueue.submit( submitInfo, nullptr );
	gQueue.waitIdle();
}

vk::ImageCreateInfo Image::getImageCreateInfo() const
{
	return vk::ImageCreateInfo{
		vk::ImageCreateFlags(),
		getImageType(),
		format,
		extent,
		1u,
		1u,
		vk::SampleCountFlagBits::e1,
	};
}

vk::DescriptorImageInfo Image::getDescriptorImageInfo() const
{
	return vk::DescriptorImageInfo{ sampler, imageView, vk::ImageLayout::eShaderReadOnlyOptimal };
}

vk::ImageType Image::getImageType() const { return vk::ImageType::e1D; }

vk::ImageViewType Image::getImageViewType( const bool arrayType ) const { return arrayType ? vk::ImageViewType::e1DArray : vk::ImageViewType::e1D; }

uint32_t Image::getMipLevels( const vk::Extent2D& extent )
{
	return static_cast<uint32_t>(std::floor( std::log2( std::max( extent.width, extent.height ) ) ) + 1);
}

uint32_t Image::getMipLevels( const vk::Extent3D& extent )
{
	return static_cast<uint32_t>(std::floor( std::log2( std::max( extent.width, std::max( extent.height, extent.depth ) ) ) ) + 1);
}

VK_NS_END
V3D_NS_END