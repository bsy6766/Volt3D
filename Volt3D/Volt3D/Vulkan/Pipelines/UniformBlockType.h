/**
*	@file UniformBlockType.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_UNIFORM_BLOCK_TYPE_H
#define V3D_UNIFORM_BLOCK_TYPE_H

#include "Utils/Macros.h"

#include <string>

V3D_NS_BEGIN
VK_NS_BEGIN

/**
*	@enum UniformBlockType
*	Defines uniform buffer type
*/
enum class UniformBlockType
{
	eUndefined = 0,
	eUniform,
	eStorage,
	ePush
};

static inline std::string uniformBlockTypeToString( v3d::vulkan::UniformBlockType value )
{
	switch (value)
	{
	case v3d::vulkan::UniformBlockType::eUniform: return "Uniform";
	case v3d::vulkan::UniformBlockType::eStorage: return "Storage";
	case v3d::vulkan::UniformBlockType::ePush: return "Push";
	default: return "Undefined";
	}
}

static inline v3d::vulkan::UniformBlockType toUniformBlockType( const unsigned int value )
{
	switch (value)
	{
	case 1: return v3d::vulkan::UniformBlockType::eUniform;
	case 2: return v3d::vulkan::UniformBlockType::eStorage;
	case 3: return v3d::vulkan::UniformBlockType::ePush;
	default: return v3d::vulkan::UniformBlockType::eUndefined;
	}
}

VK_NS_END
V3D_NS_END

#endif