/**
*	@file Image2D.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_IMAGE_2D_H
#define V3D_VK_IMAGE_2D_H

#include <vulkan/vulkan.hpp>

#include "Image.h"
#include "Utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class VOLT3D_DLL Image2D : public v3d::vulkan::Image
{
	friend class Context;
	friend class Texture2D;

private:
	Image2D( const vk::Extent3D& extent, const vk::Format& format );

public:
	~Image2D();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Image2D );
	DEFAULT_MOVE_CONSTRUCTORS( Image2D );

	virtual vk::ImageType getImageType() const override;
	virtual vk::ImageViewType getImageViewType( const bool arrayType = false ) const override;
};

VK_NS_END
V3D_NS_END

#endif