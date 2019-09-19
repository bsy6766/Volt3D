/**
*	@file Texture.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Texture.h"

#include "Images/Image.h"
#include "Images/Image2D.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Texture::Texture()
	: image(nullptr)
{}

Texture::~Texture() {}

Texture* v3d::vulkan::Texture::create2D( const vk::Extent2D& extent, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags memoryProperty )
{
	v3d::vulkan::Texture* newTexture = new (std::nothrow) v3d::vulkan::Texture();
	if (newTexture)
	{
		if (newTexture->init())
		{

		}
	}

	return nullptr;
}

bool Texture::init2D()
{
	image = new (std::nothrow) v3d::vulkan::Image2D();
}

void Texture::release()
{
}


VK_NS_END
V3D_NS_END;