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

Image2D::Image2D()
	: v3d::vulkan::Image()
{
	type = vk::ImageType::e2D;
}

Image2D::~Image2D()
{
}


VK_NS_END
V3D_NS_END