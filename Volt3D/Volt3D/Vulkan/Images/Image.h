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

protected:
	Image();

	const vk::Device& logicalDevice;

	vk::Image image;
	vk::DeviceMemory deviceMemory;
	vk::Sampler sampler;
	vk::ImageLayout imageLayout;
	vk::ImageView imageView;

	vk::Extent3D extent;
	vk::Format format;
	//vk::Filter filter;
	//vk::ImageUsageFlagBits usageFlagBits;

	uint32_t mip_levels;
	uint32_t layers;

	//VkSampleCountFlagBits sampleCount;
	//VkSamplerAddressMode samplerAddressMode;
	//bool anisotropic;

	void initImage( const uint32_t width, const uint32_t height, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags );
	void initImageDeviceMemory( const vk::MemoryPropertyFlags memoryPropertyFlags );
	void initSampler();
	void transitionLayout( const vk::ImageLayout oldLayout, const vk::ImageLayout newLayout );
	void copyBuffer();

public:
	virtual ~Image();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Image );
	DEFAULT_MOVE_CONSTRUCTORS( Image );

	//const vk::Image& getImage() const;
	//const vk::DeviceMemory& getDeviceMemory() const;
	//const vk::Sampler& getSampler() const;

	static uint32_t get_mip_levels( const vk::Extent2D& extent );
	static uint32_t get_mip_levels( const vk::Extent3D& extent );
};

VK_NS_END
V3D_NS_END

#endif