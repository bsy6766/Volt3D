/**
*	@file ShaderModule.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_SHADER_MODULE_H
#define V3D_VK_SHADER_MODULE_H

#include <vulkan/vulkan.hpp>

#include <glslang/Public/ShaderLang.h>

#include "utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class ShaderModule
{
	friend class Pipeline;

private:
	ShaderModule();

	vk::ShaderModule shaderModule;

	bool compile( const std::vector<char>& sourceCode );

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( ShaderModule );
	DEFAULT_MOVE_CONSTRUCTORS( ShaderModule );

	~ShaderModule();

	vk::ShaderModule get() const;
};

VK_NS_END
V3D_NS_END

#endif