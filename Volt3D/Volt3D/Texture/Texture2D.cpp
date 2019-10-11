/**
*	@file Texture2D.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Texture2D.h"

#include "Vulkan/Images/Image2D.h"

V3D_NS_BEGIN


Texture2D::Texture2D()
	: v3d::Texture()
{}

Texture2D::Texture2D( const std::string & name )
	: v3d::Texture(name)
{}


v3d::Texture2D::~Texture2D() {}

Texture2D* Texture2D::create( const std::string& name, const std::filesystem::path& textureFilePath, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags memoryProperty )
{
	v3d::Texture2D* newTexture = new (std::nothrow) v3d::Texture2D( name );
	if (newTexture)
	{
		if (newTexture->init( textureFilePath, tilling, usage, memoryProperty )) return newTexture;
		SAFE_DELETE( newTexture );
	}

	return nullptr;
}
bool Texture2D::initImage( const vk::Extent3D& extent, const vk::Format& format )
{
	image = new (std::nothrow) v3d::vulkan::Image2D( extent, format );
	return image != nullptr;
}

V3D_NS_END
