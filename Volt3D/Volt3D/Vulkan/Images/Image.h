/**
*	@file Image.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_IMAGE_H
#define V3D_VK_IMAGE_H

#include <vulkan/vulkan.hpp>

#include "Utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class VOLT3D_DLL Image
{
	friend class Context;
	friend class Texture;

protected:
	Image( const vk::Extent3D& extent, const vk::Format& format );

	const vk::Device& logicalDevice;

	vk::Image image;
	vk::DeviceMemory deviceMemory;
	vk::Sampler sampler;
	vk::ImageLayout imageLayout;
	vk::ImageView imageView;

	vk::Extent3D extent;
	vk::Format format;
	vk::ImageType type;
	//vk::Filter filter;
	//vk::ImageUsageFlagBits usageFlagBits;

	uint32_t mip_levels;
	uint32_t layers;

	//VkSampleCountFlagBits sampleCount;
	//VkSamplerAddressMode samplerAddressMode;
	//bool anisotropic;

	void initImage( const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags );
	void initImageDeviceMemory( const vk::MemoryPropertyFlags memoryPropertyFlags );
	void initImageView();
	void initSampler();
	void transitionLayout( const vk::ImageLayout oldLayout, const vk::ImageLayout newLayout, const vk::PipelineStageFlags srcStage, const vk::PipelineStageFlags dstStage );
	void copyBuffer( const vk::Buffer& stagingBuffer );

	vk::ImageCreateInfo getImageCreateInfo() const;
	virtual vk::ImageType getImageType() const;
	virtual vk::ImageViewType getImageViewType( const bool arrayType = false ) const;

public:
	virtual ~Image();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Image );
	DEFAULT_MOVE_CONSTRUCTORS( Image );

	//const vk::Image& getImage() const;
	//const vk::DeviceMemory& getDeviceMemory() const;
	//const vk::Sampler& getSampler() const;

	uint32_t getWidth() const { return extent.width; }
	uint32_t getHeight() const { return extent.height; }
	uint32_t getDepth() const { return extent.depth; }

	const vk::Sampler& getSampler() const { return sampler; }
	const vk::ImageView& getImageview() const { return imageView; }

	static uint32_t getMipLevels( const vk::Extent2D& extent );
	static uint32_t getMipLevels( const vk::Extent3D& extent );
};

VK_NS_END
V3D_NS_END

#endif