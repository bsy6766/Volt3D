/**
*	@file Texture.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_TEXTURE_H
#define V3D_VK_TEXTURE_H

#include <vulkan/vulkan.hpp>

#include "Utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class Image;

/**
*	@class Texture
*	@brief A class that load texture to vulkan texture (image, imageView, etc...)
*
*	@group vulkan
*
*	@since 1.0
*/
class Texture
{
	friend class Context;

protected:
	Texture();

	v3d::vulkan::Image* image;

	void release();

	bool init2D( const std::string& texture_name, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags memoryProperty );
	//bool init( const vk::Extent3D& extent, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags memoryProperty );

public:
	~Texture();

	//static Texture* create1D( const vk::Extent2D& extent );
	static Texture* create2D( const std::string& texture_name, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags memoryProperty );
	//static Texture* create3D();
	//static Texture* create1DArray();
	//static Texture* create2DArray();
	//static Texture* createCubeMap();

	uint32_t getWidth() const;
	uint32_t getHeight() const;
	uint32_t getDepth() const;
	v3d::vulkan::Image* getImage() const;
};

VK_NS_END
V3D_NS_END;

#endif