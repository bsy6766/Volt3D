/**
*	@file ShaderStageBuilder.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_SHADER_STAGE_BUILDER_H
#define V3D_VK_SHADER_STAGE_BUILDER_H

#include <vulkan/vulkan.hpp>

#include <array>
#include <memory>

#include "utils/Macros.h"

V3D_NS_BEGIN
class Shader;
VK_NS_BEGIN
class Pipeline;

class VOLT3D_DLL ShaderStages
{

};

class VOLT3D_DLL ShaderStageBuilder
{
private:
	ShaderStageBuilder();
public:

	//std::array<std::shared_ptr<v3d::Shader>, 2> create()
};

VK_NS_END
V3D_NS_END

#endif