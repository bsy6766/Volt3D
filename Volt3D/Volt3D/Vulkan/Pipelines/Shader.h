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

	const vk::Device& logicalDevice;
	std::filesystem::path filePath;
	vk::ShaderStageFlagBits stage;
	vk::ShaderModule shaderModule;

	bool init();

	EShLanguage getEShLanguage() const;
	std::vector<char> readFile( const std::filesystem::path& filePath );

	std::unordered_map<uint32_t, v3d::vulkan::UniformBlock> uniformBlocks;
	std::unordered_map<uint32_t, v3d::vulkan::Uniform> uniforms;

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Shader );
	DEFAULT_MOVE_CONSTRUCTORS( Shader );

	~Shader();


	/** Get Vulkan ShaderModule */
	const vk::ShaderModule get() const;

	/** Get Vulkan PipelineShaderStageCreateInfo */
	vk::PipelineShaderStageCreateInfo getPipelineShaderStageCreateInfo() const;

	/**
	*	Get uniform block by binding
	*	@param binding A uniform block binding defined in shader.
	*	@return A pointer to uniform block with matching binding. Else, std::nullopt.
	*/
	std::optional<std::reference_wrapper<v3d::vulkan::UniformBlock>> getUniformBlock( const uint32_t binding );

	/**
	*	Get uniform block by name
	*	@param binding A uniform block name defined in shader.
	*	@return A uniform block with matching name. Else, std::nullopt.
	*/
	std::optional<std::reference_wrapper<v3d::vulkan::UniformBlock>> getUniformBlock( const std::string_view name );

	/** Get shader stage flag bits */
	vk::ShaderStageFlagBits getStage() const;

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