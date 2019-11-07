/**
*	@file DepthImage.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_DEPTH_IMAGE_H
#define V3D_VK_DEPTH_IMAGE_H

#include <vulkan/vulkan.hpp>

#include "Utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class DepthImage
{
	friend class Swapchain;

private:
	DepthImage();

	vk::Image image;
	vk::ImageView imageView;
	vk::DeviceMemory deviceMemory;

	bool init( const vk::Extent2D& extent, const vk::Format& format, const vk::ImageTiling tiling = vk::ImageTiling::eOptimal, const vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eDepthStencilAttachment, const vk::MemoryPropertyFlags memProperties = vk::MemoryPropertyFlagBits::eDeviceLocal );

public:
	~DepthImage();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( DepthImage );
	DEFAULT_MOVE_CONSTRUCTORS( DepthImage );

	const vk::ImageView& getImageView() const;
};

VK_NS_END
V3D_NS_END

#endif