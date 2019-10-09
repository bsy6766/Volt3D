/**
*	@file Image2D.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Image2D.h"

#include "Vulkan/Context.h"
#include "Vulkan/Devices/LogicalDevice.h"
#include "Vulkan/Devices/PhysicalDevice.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Image2D::Image2D( const vk::Extent3D& extent, const vk::Format& format )
	: v3d::vulkan::Image( extent, format )
{}

Image2D::~Image2D() {}

vk::ImageType Image2D::getImageType() const { return vk::ImageType::e2D; }

vk::ImageViewType Image2D::getImageViewType( const bool arrayType ) const { return arrayType ? vk::ImageViewType::e2DArray : vk::ImageViewType::e2D ; }


VK_NS_END
V3D_NS_END