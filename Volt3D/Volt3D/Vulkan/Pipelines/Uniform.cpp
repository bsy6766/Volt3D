/**
*	@file Uniform.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Uniform.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Uniform::Uniform( const std::string& name )
	: name( name )
	, binding( 0 )
	, offset( 0 )
	, size( 0 )
	, glType( 0 )
	, value()
{}

Uniform::~Uniform() {}

std::string Uniform::getName() const { return name; }

inline int32_t Uniform::getBinding() const { return binding; }

inline int32_t Uniform::getOffset() const { return offset; }

inline int32_t Uniform::getSize() const { return size; }

inline int32_t Uniform::getGLType() const { return glType; }

inline vk::Format Uniform::getFormat() const
{
	switch (glType)
	{
	case 0x1406: // GL_FLOAT
		return vk::Format::eR32Sfloat;
	case 0x8B50: // GL_FLOAT_VEC2
		return vk::Format::eR32G32Sfloat;
	case 0x8B51: // GL_FLOAT_VEC3
		return vk::Format::eR32G32B32Sfloat;
	case 0x8B52: // GL_FLOAT_VEC4
		return vk::Format::eR32G32B32A32Sfloat;
	case 0x1404: // GL_INT
		return vk::Format::eR32Sint;
	case 0x8B53: // GL_INT_VEC2
		return vk::Format::eR32G32Sint;
	case 0x8B54: // GL_INT_VEC3
		return vk::Format::eR32G32B32Sint;
	case 0x8B55: // GL_INT_VEC4
		return vk::Format::eR32G32B32A32Sint;
	case 0x1405: // GL_UNSIGNED_INT
		return vk::Format::eR32Uint;
	case 0x8DC6: // GL_UNSIGNED_INT_VEC2
		return vk::Format::eR32G32Uint;
	case 0x8DC7: // GL_UNSIGNED_INT_VEC3
		return vk::Format::eR32G32B32Uint;
	case 0x8DC8: // GL_UNSIGNED_INT_VEC4
		return vk::Format::eR32G32B32A32Uint;
	default:
		return vk::Format::eUndefined;
	}
}

VK_NS_END
V3D_NS_END
