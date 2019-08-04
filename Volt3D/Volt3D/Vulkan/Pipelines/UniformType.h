/**
*	@file UniformType.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_UNIFORM_TYPE_H
#define V3D_UNIFORM_TYPE_H

#include "Utils/Macros.h"

#include <string>

V3D_NS_BEGIN
VK_NS_BEGIN

/**
*	@enum UniformType
*	Defines uniform data type
*/
enum class UniformType
{
	eUndefined = 0,

	eFloat = 0x1406,
	eVec2 = 0x8B50,
	eVec3 = 0x8B51,
	eVec4 = 0x8B52,

	eInt = 0x1404,
	eIVec2 = 0x8B53,
	eIVec3 = 0x8B54,
	eIVec4 = 0x8B55,

	eUint = 0x1405,
	eUVec2 = 0x8DC6,
	eUVec3 = 0x8DC7,
	eUVec4 = 0x8DC8,

	eBool = 0x8B56,
	eBVec2 = 0x8B57,
	eBVec3 = 0x8B58,
	eBVec4 = 0x8B59,

	eMat2 = 0x8B5A,
	eMat3 = 0x8B5B,
	eMat4 = 0x8B5C,
	eMat2x3 = 0x8B65,
	eMat2x4 = 0x8B66,
	eMat3x2 = 0x8B67,
	eMat3x4 = 0x8B68,
	eMat4x2 = 0x8B69,
	eMat4x3 = 0x8B6A,
};

static inline std::string uniformTypeToString( v3d::vulkan::UniformType value )
{
	switch (value)
	{
	case v3d::vulkan::UniformType::eFloat: return "Float";
	case v3d::vulkan::UniformType::eVec2: return "Vec2";
	case v3d::vulkan::UniformType::eVec3: return "Vec3";
	case v3d::vulkan::UniformType::eVec4: return "Vec4";
	case v3d::vulkan::UniformType::eInt: return "Int";
	case v3d::vulkan::UniformType::eIVec2: return "IVec2";
	case v3d::vulkan::UniformType::eIVec3: return "IVec3";
	case v3d::vulkan::UniformType::eIVec4: return "IVec4";
	case v3d::vulkan::UniformType::eUint: return "Uint";
	case v3d::vulkan::UniformType::eUVec2: return "UVec2";
	case v3d::vulkan::UniformType::eUVec3: return "UVec3";
	case v3d::vulkan::UniformType::eUVec4: return "UVec4";
	case v3d::vulkan::UniformType::eBool: return "Bool";
	case v3d::vulkan::UniformType::eBVec2: return "BVec2";
	case v3d::vulkan::UniformType::eBVec3: return "BVec3";
	case v3d::vulkan::UniformType::eBVec4: return "BVec4";
	case v3d::vulkan::UniformType::eMat2: return "Mat2";
	case v3d::vulkan::UniformType::eMat3: return "Mat3";
	case v3d::vulkan::UniformType::eMat4: return "Mat4";
	case v3d::vulkan::UniformType::eMat2x3: return "Mat2x3";
	case v3d::vulkan::UniformType::eMat2x4: return "Mat2x4";
	case v3d::vulkan::UniformType::eMat3x2: return "Mat3x2";
	case v3d::vulkan::UniformType::eMat3x4: return "Mat3x4";
	case v3d::vulkan::UniformType::eMat4x2: return "Mat4x2";
	case v3d::vulkan::UniformType::eMat4x3: return "Mat4x3";
	default: return "Undefined";
	}
}

static inline v3d::vulkan::UniformType toUniformType( const unsigned int value )
{
	switch (value)
	{
	case 0x1406: return v3d::vulkan::UniformType::eFloat;
	case 0x8B50: return v3d::vulkan::UniformType::eVec2;
	case 0x8B51: return v3d::vulkan::UniformType::eVec3;
	case 0x8B52: return v3d::vulkan::UniformType::eVec4;
	case 0x1404: return v3d::vulkan::UniformType::eInt;
	case 0x8B53: return v3d::vulkan::UniformType::eIVec2;
	case 0x8B54: return v3d::vulkan::UniformType::eIVec3;
	case 0x8B55: return v3d::vulkan::UniformType::eIVec4;
	case 0x1405: return v3d::vulkan::UniformType::eUint;
	case 0x8DC6: return v3d::vulkan::UniformType::eUVec2;
	case 0x8DC7: return v3d::vulkan::UniformType::eUVec3;
	case 0x8DC8: return v3d::vulkan::UniformType::eUVec4;
	case 0x8B56: return v3d::vulkan::UniformType::eBool;
	case 0x8B57: return v3d::vulkan::UniformType::eBVec2;
	case 0x8B58: return v3d::vulkan::UniformType::eBVec3;
	case 0x8B59: return v3d::vulkan::UniformType::eBVec4;
	case 0x8B5A: return v3d::vulkan::UniformType::eMat2;
	case 0x8B5B: return v3d::vulkan::UniformType::eMat3;
	case 0x8B5C: return v3d::vulkan::UniformType::eMat4;
	case 0x8B65: return v3d::vulkan::UniformType::eMat2x3;
	case 0x8B66: return v3d::vulkan::UniformType::eMat2x4;
	case 0x8B67: return v3d::vulkan::UniformType::eMat3x2;
	case 0x8B68: return v3d::vulkan::UniformType::eMat3x4;
	case 0x8B69: return v3d::vulkan::UniformType::eMat4x2;
	case 0x8B6A: return v3d::vulkan::UniformType::eMat4x3;
	default: return v3d::vulkan::UniformType::eUndefined;
	}
}

VK_NS_END
V3D_NS_END

#endif