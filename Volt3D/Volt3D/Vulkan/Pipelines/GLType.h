/**
*	@file GLType.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_GL_DATA_TYPE_H
#define V3D_GL_DATA_TYPE_H

#include "Utils/Macros.h"

#include <string>

V3D_NS_BEGIN
VK_NS_BEGIN

/**
*	@enum GLType
*	Defines uniform data type
*/
enum class GLType
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

	eSampler1D = 0x8B5D,
	eSampler2D = 0x8B5E,
	eSampler3D = 0x8B5F,
	// @todo add more type
	//#define GL_SAMPLER_CUBE                   0x8B60
	//#define GL_SAMPLER_BUFFER                 0x8DC2
	//#define GL_SAMPLER_1D_ARRAY               0x8DC0
	//#define GL_SAMPLER_2D_ARRAY               0x8DC1
	//#define GL_SAMPLER_1D_ARRAY_SHADOW        0x8DC3
	//#define GL_SAMPLER_2D_ARRAY_SHADOW        0x8DC4
	//#define GL_SAMPLER_CUBE_SHADOW            0x8DC5
	//#define GL_SAMPLER_1D_SHADOW              0x8B61
	//#define GL_SAMPLER_2D_SHADOW              0x8B62
	//#define GL_SAMPLER_2D_RECT                0x8B63
	//#define GL_SAMPLER_2D_RECT_SHADOW         0x8B64
	//#define GL_SAMPLER_2D_MULTISAMPLE         0x9108
	//#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY   0x910B
	//#define GL_SAMPLER_CUBE_MAP_ARRAY         0x900C
	//#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW  0x900D
	//#define GL_SAMPLER_CUBE_MAP_ARRAY_ARB     0x900C
	//#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB 0x900D
};

static inline std::string GLDataTypeToString( v3d::vulkan::GLType value )
{
	switch (value)
	{
	case v3d::vulkan::GLType::eFloat: return "Float";
	case v3d::vulkan::GLType::eVec2: return "Vec2";
	case v3d::vulkan::GLType::eVec3: return "Vec3";
	case v3d::vulkan::GLType::eVec4: return "Vec4";
	case v3d::vulkan::GLType::eInt: return "Int";
	case v3d::vulkan::GLType::eIVec2: return "IVec2";
	case v3d::vulkan::GLType::eIVec3: return "IVec3";
	case v3d::vulkan::GLType::eIVec4: return "IVec4";
	case v3d::vulkan::GLType::eUint: return "Uint";
	case v3d::vulkan::GLType::eUVec2: return "UVec2";
	case v3d::vulkan::GLType::eUVec3: return "UVec3";
	case v3d::vulkan::GLType::eUVec4: return "UVec4";
	case v3d::vulkan::GLType::eBool: return "Bool";
	case v3d::vulkan::GLType::eBVec2: return "BVec2";
	case v3d::vulkan::GLType::eBVec3: return "BVec3";
	case v3d::vulkan::GLType::eBVec4: return "BVec4";
	case v3d::vulkan::GLType::eMat2: return "Mat2";
	case v3d::vulkan::GLType::eMat3: return "Mat3";
	case v3d::vulkan::GLType::eMat4: return "Mat4";
	case v3d::vulkan::GLType::eMat2x3: return "Mat2x3";
	case v3d::vulkan::GLType::eMat2x4: return "Mat2x4";
	case v3d::vulkan::GLType::eMat3x2: return "Mat3x2";
	case v3d::vulkan::GLType::eMat3x4: return "Mat3x4";
	case v3d::vulkan::GLType::eMat4x2: return "Mat4x2";
	case v3d::vulkan::GLType::eMat4x3: return "Mat4x3";
	case v3d::vulkan::GLType::eSampler1D: return "Sampler1D";
	case v3d::vulkan::GLType::eSampler2D: return "Sampler2D";
	case v3d::vulkan::GLType::eSampler3D: return "Sampler3D";
	default: return "Undefined";
	}
}

static inline v3d::vulkan::GLType toGLTypeEnum( const unsigned int value )
{
	switch (value)
	{
	case 0x1406: return v3d::vulkan::GLType::eFloat;
	case 0x8B50: return v3d::vulkan::GLType::eVec2;
	case 0x8B51: return v3d::vulkan::GLType::eVec3;
	case 0x8B52: return v3d::vulkan::GLType::eVec4;
	case 0x1404: return v3d::vulkan::GLType::eInt;
	case 0x8B53: return v3d::vulkan::GLType::eIVec2;
	case 0x8B54: return v3d::vulkan::GLType::eIVec3;
	case 0x8B55: return v3d::vulkan::GLType::eIVec4;
	case 0x1405: return v3d::vulkan::GLType::eUint;
	case 0x8DC6: return v3d::vulkan::GLType::eUVec2;
	case 0x8DC7: return v3d::vulkan::GLType::eUVec3;
	case 0x8DC8: return v3d::vulkan::GLType::eUVec4;
	case 0x8B56: return v3d::vulkan::GLType::eBool;
	case 0x8B57: return v3d::vulkan::GLType::eBVec2;
	case 0x8B58: return v3d::vulkan::GLType::eBVec3;
	case 0x8B59: return v3d::vulkan::GLType::eBVec4;
	case 0x8B5A: return v3d::vulkan::GLType::eMat2;
	case 0x8B5B: return v3d::vulkan::GLType::eMat3;
	case 0x8B5C: return v3d::vulkan::GLType::eMat4;
	case 0x8B65: return v3d::vulkan::GLType::eMat2x3;
	case 0x8B66: return v3d::vulkan::GLType::eMat2x4;
	case 0x8B67: return v3d::vulkan::GLType::eMat3x2;
	case 0x8B68: return v3d::vulkan::GLType::eMat3x4;
	case 0x8B69: return v3d::vulkan::GLType::eMat4x2;
	case 0x8B6A: return v3d::vulkan::GLType::eMat4x3;
	case 0x8B5D: return v3d::vulkan::GLType::eSampler1D;
	case 0x8B5E: return v3d::vulkan::GLType::eSampler2D;
	case 0x8B5F: return v3d::vulkan::GLType::eSampler3D;
	default: return v3d::vulkan::GLType::eUndefined;
	}
}

VK_NS_END
V3D_NS_END

#endif