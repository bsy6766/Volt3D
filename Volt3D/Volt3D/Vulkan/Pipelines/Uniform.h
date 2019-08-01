/**
*	@file Uniform.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_UNIFORM_H
#define V3D_VK_UNIFORM_H

#include "utils/Macros.h"

#include <Vulkan/vulkan.hpp>

#include <vector>

V3D_NS_BEGIN
VK_NS_BEGIN

class VOLT3D_DLL Uniform
{
	friend class Shader;

public:
	union Value
	{
	public:
		bool boolValue;
		int intValue;
		float floatValue;
		float v2Value[2];
		float v3Value[3];
		float v4Value[4];
		float mat4Value[16];
	};

private:
	Uniform() = delete;
	Uniform( const std::string& name );

	std::string name;
	v3d::vulkan::Uniform::Value value;

	int32_t binding;
	int32_t offset;
	int32_t size;
	int32_t glType;

public:
	~Uniform();

	inline std::string getName() const;

	inline int32_t getBinding() const;
	inline int32_t getOffset() const;
	inline int32_t getSize() const;
	inline int32_t getGLType() const;
	inline vk::Format getFormat() const;

	inline bool isBool() const;
	inline bool isInt() const;
	inline bool isFloat() const;
	inline bool isVec2() const;
	inline bool isVec3() const;
	inline bool isVec4() const;
	inline bool isMat4() const;
};

VK_NS_END
V3D_NS_END

#endif