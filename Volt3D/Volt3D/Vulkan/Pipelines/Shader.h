/**
*	@file Shader.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_SHADER_H
#define V3D_VK_SHADER_H

#include <vulkan/vulkan.hpp>

#include <unordered_map>
#include <filesystem>
#include <optional>

#include <glslang/Public/ShaderLang.h>

#include "ShaderState.h"
#include "utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class Pipeline;

/**
*	@class Shader
*	@brief A wrapper class for Vulkan's ShaderModule
*	@note Compiles shader in runtime using glslang.
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL Shader
{
	friend class Pipeline;

private:
	Shader() = delete;
	Shader( const std::filesystem::path& filePath );

	std::filesystem::path filePath;

	vk::ShaderStageFlagBits stage;
	vk::ShaderModule shaderModule;
	v3d::vulkan::ShaderState shaderState;

	bool compile();

	EShLanguage getEShLanguage() const;

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Shader );
	DEFAULT_MOVE_CONSTRUCTORS( Shader );

	~Shader();

	/** Get Vulkan ShaderModule */
	const vk::ShaderModule getShaderModule() const;

	/** Get Vulkan PipelineShaderStageCreateInfo */
	vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo() const;

	/** Get shader stage flag bits */
	vk::ShaderStageFlagBits getStage() const;

	std::vector<vk::DescriptorSetLayoutBinding> getDescriptorSetLayoutBinding() const;

	/**
	*	Get shader stage flag bits
	*	@param fileName A file name of shader file.
	*	@return Vulkan ShaderStageFlagBits based on shader file extension. Returns vk::ShaderStageFlagBits::eAll if none of matches.
	*/
	static const vk::ShaderStageFlagBits toShaderStageFlagbits( const std::filesystem::path& fileName );
};

VK_NS_END
V3D_NS_END

#endif