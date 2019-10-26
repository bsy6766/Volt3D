/**
*	@file ShaderModule.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_SHADER_MODULE_H
#define V3D_VK_SHADER_MODULE_H

#include <vulkan/vulkan.hpp>

#include <unordered_map>
#include <filesystem>
#include <optional>

#include <glslang/Public/ShaderLang.h>

#include "utils/Macros.h"

V3D_NS_BEGIN
class ShaderState;

VK_NS_BEGIN
class Pipeline;

/**
*	@class ShaderModule
*	@brief A wrapper class for Vulkan's ShaderModule
*	@note Compiles shader in runtime using glslang.
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL ShaderModule
{
	friend class Pipeline;
	friend class Shader;

private:
	ShaderModule();

	vk::ShaderStageFlagBits stage;
	vk::ShaderModule shaderModule;
	
	bool compile( const std::vector<char>& sourceCode, const std::filesystem::path& filePath, v3d::ShaderState& shaderState );

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( ShaderModule );
	DEFAULT_MOVE_CONSTRUCTORS( ShaderModule );

	~ShaderModule();

	/** Get Vulkan ShaderModule */
	inline const vk::ShaderModule getShaderModule() const;

	/** Get Vulkan PipelineShaderStageCreateInfo */
	inline vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo() const;

	/** Get shader stage flag bits */
	inline vk::ShaderStageFlagBits getStage() const;
};

VK_NS_END
V3D_NS_END

#endif