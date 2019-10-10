/**
*	@file Texture.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_TEXTURE_H
#define V3D_VK_TEXTURE_H

#include <memory>
#include <unordered_map>

#include <vulkan/vulkan.hpp>

#include "Utils/Macros.h"

V3D_NS_BEGIN

VK_NS_BEGIN
class Image;
VK_NS_END

/**
*	@class Texture
*	@brief A class that load texture to vulkan texture (image, imageView, etc...)
*
*	@group vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL Texture
{
	friend class Context;
	friend class TextureManager;

protected:
	Texture();

	v3d::vulkan::Image* image;

	void release();

	virtual bool initImage( const vk::Extent3D& extent, const vk::Format& format );
	bool init( const std::string& texture_name, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags memoryProperty );

public:
	virtual ~Texture();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Texture );
	DEFAULT_MOVE_CONSTRUCTORS( Texture );

	static Texture* create( const std::string& texture_name, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usage, const vk::MemoryPropertyFlags memoryProperty );

	/** Get width of texture */
	uint32_t getWidth() const;

	/** Get height of texture */
	uint32_t getHeight() const;

	/** Get depth of texture */
	uint32_t getDepth() const;

	/** Get image ptr */
	v3d::vulkan::Image* getImage() const;
};

V3D_NS_END;

#endif