/**
*	@file Uniform.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_UNIFORM_H
#define V3D_VK_UNIFORM_H

#include "utils/Macros.h"

#include <vector>

#include "UniformValue.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class VOLT3D_DLL Uniform
{
private:
	friend class Shader;

	Uniform();

	std::vector<v3d::vulkan::UniformValue> uniformValues;

	int32_t binding;
	int32_t offset;
	int32_t size;
	int32_t glType;

public:
	~Uniform();
};

VK_NS_END
V3D_NS_END

#endif