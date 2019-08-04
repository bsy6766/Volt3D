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

#include "UniformBlock.h"
#include "utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

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
private:
	const vk::Device& logicalDevice;
	std::filesystem::path filePath;
	vk::ShaderStageFlagBits stage;
	vk::ShaderModule shaderModule;

	EShLanguage getEShLanguage() const;
	std::vector<char> readFile( const std::filesystem::path& filePath );

	std::unordered_map<uint32_t, v3d::vulkan::UniformBlock> uniformBlocks;
	std::unordered_map<uint32_t, v3d::vulkan::Uniform> uniforms;

public:
	Shader() = delete;
	Shader(const std::filesystem::path& filePath);
	~Shader();
	
	bool init( const std::filesystem::path& filePath );

	/** Get Vulkan ShaderModule */
	const vk::ShaderModule get() const;

	/** Get Vulkan PipelineShaderStageCreateInfo */
	vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo() const;

	std::optional<v3d::vulkan::UniformBlock> getUniformBlock( const uint32_t binding );

	std::optional<v3d::vulkan::UniformBlock> getUniformBlock( const std::string_view name );

	/**
	*	Get shader stage flag bits
	*	@param fileName A file name of shader file.
	*	@return Vulkan ShaderStageFlagBits based on shader file extension. Returns vk::ShaderStageFlagBits::eAll if none of matches.
	*/
	static const vk::ShaderStageFlagBits getShaderStage( const std::filesystem::path& fileName );
};

VK_NS_END
V3D_NS_END

#endif