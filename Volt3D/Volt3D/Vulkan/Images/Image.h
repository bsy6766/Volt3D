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
private:
	Image();

	const vk::Device& logicalDevice;

	vk::Extent3D extent;
	vk::Format format;
	vk::ImageUsageFlagBits usageFlagBits;

	uint32_t mip_levels;
	uint32_t layers;

	vk::Filter filter;

	vk::Image image;
	vk::DeviceMemory imageDeviceMemory;
	vk::Sampler sampler;
	vk::ImageLayout imageLayout;
	vk::ImageView imageView;

	VkSampleCountFlagBits m_samples;
	VkSamplerAddressMode m_addressMode;
	bool m_anisotropic;

	void initImage( const uint32_t w, const uint32_t h, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags );
	void initImageDeviceMemory( const vk::MemoryPropertyFlags memoryPropertyFlags );


public:
	~Image();
};

VK_NS_END
V3D_NS_END

#endif