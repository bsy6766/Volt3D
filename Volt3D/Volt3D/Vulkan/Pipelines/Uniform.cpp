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

inline v3d::vulkan::Uniform::Type Uniform::getDataType() const
{
	switch (glType)
	{
	case 0x1406: return v3d::vulkan::Uniform::Type::eFloat;
	case 0x8B50: return v3d::vulkan::Uniform::Type::eVec2;
	case 0x8B51: return v3d::vulkan::Uniform::Type::eVec3;
	case 0x8B52: return v3d::vulkan::Uniform::Type::eVec4;
	case 0x1404: return v3d::vulkan::Uniform::Type::eInt;
	case 0x8B53: return v3d::vulkan::Uniform::Type::eIVec2;
	case 0x8B54: return v3d::vulkan::Uniform::Type::eIVec3;
	case 0x8B55: return v3d::vulkan::Uniform::Type::eIVec4;
	case 0x1405: return v3d::vulkan::Uniform::Type::eUint;
	case 0x8DC6: return v3d::vulkan::Uniform::Type::eUVec2;
	case 0x8DC7: return v3d::vulkan::Uniform::Type::eUVec3;
	case 0x8DC8: return v3d::vulkan::Uniform::Type::eUVec4;
	case 0x8B56: return v3d::vulkan::Uniform::Type::eBool;
	case 0x8B57: return v3d::vulkan::Uniform::Type::eBVec2;
	case 0x8B58: return v3d::vulkan::Uniform::Type::eBVec3;
	case 0x8B59: return v3d::vulkan::Uniform::Type::eBVec4;
	case 0x8B5A: return v3d::vulkan::Uniform::Type::eMat2;
	case 0x8B5B: return v3d::vulkan::Uniform::Type::eMat3;
	case 0x8B5C: return v3d::vulkan::Uniform::Type::eMat4;
	case 0x8B65: return v3d::vulkan::Uniform::Type::eMat2x3;
	case 0x8B66: return v3d::vulkan::Uniform::Type::eMat2x4;
	case 0x8B67: return v3d::vulkan::Uniform::Type::eMat3x2;
	case 0x8B68: return v3d::vulkan::Uniform::Type::eMat3x4;
	case 0x8B69: return v3d::vulkan::Uniform::Type::eMat4x2;
	case 0x8B6A: return v3d::vulkan::Uniform::Type::eMat4x3;
	default: return v3d::vulkan::Uniform::Type::eUndefined;
	}
}

inline bool Uniform::isFloat() const { return glType == 0x1406; }

inline bool Uniform::isVec2() const { return glType == 0x8B50; }

inline bool Uniform::isVec3() const { return glType == 0x8B51; }

inline bool Uniform::isVec4() const { return glType == 0x8B52; }

inline bool Uniform::isInt() const { return glType == 0x1404; }

inline bool Uniform::isIVec2() const { return glType == 0x8B53; }

inline bool Uniform::isIVec3() const { return glType == 0x8B54; }

inline bool Uniform::isIVec4() const { return glType == 0x8B55; }

inline bool Uniform::isUint() const { return glType == 0x1405; }

inline bool Uniform::isUVec2() const { return glType == 0x8DC6; }

inline bool Uniform::isUVec3() const { return glType == 0x8DC7; }

inline bool Uniform::isUVec4() const { return glType == 0x8DC8; }

inline bool Uniform::isBool() const { return glType == 0x8B56; }

inline bool Uniform::isBVec2() const { return glType == 0x8B57; }

inline bool Uniform::isBVec3() const { return glType == 0x8B58; }

inline bool Uniform::isBVec4() const { return glType == 0x8B59; }

inline bool Uniform::isMat2() const { return glType == 0x8B5A; }

inline bool Uniform::isMat3() const { return glType == 0x8B5B; }

inline bool Uniform::isMat4() const { return glType == 0x8B5C; }

inline bool Uniform::isMat2x3() const { return glType == 0x8B65; }

inline bool Uniform::isMat2x4() const { return glType == 0x8B66; }

inline bool Uniform::isMat3x2() const { return glType == 0x8B67; }

inline bool Uniform::isMat3x4() const { return glType == 0x8B68; }

inline bool Uniform::isMat4x2() const { return glType == 0x8B69; }

inline bool Uniform::isMat4x3() const { return glType == 0x8B6A; }

VK_NS_END
V3D_NS_END
