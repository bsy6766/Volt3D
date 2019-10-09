/**
*	@file Texture2D.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_TEXTURE_2D_H
#define V3D_VK_TEXTURE_2D_H

#include <vulkan/vulkan.hpp>

#include "Texture.h"
#include "Utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

/**
*	@class Texture2D
*	@brief A class that load 2D texture to vulkan texture (image, imageView, etc...)
*
*	@group vulkan
*
*	@since 1.0
*/
class Texture2D : public v3d::vulkan::Texture
{
	friend class Context;

protected:
	Texture2D();

	virtual bool initImage( const vk::Extent3D& extent, const vk::Format& format ) override;

public:
	~Texture2D();

	static Texture2D* create( const std::string& texture_name, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags memoryProperty );
};

VK_NS_END
V3D_NS_END;

#endif