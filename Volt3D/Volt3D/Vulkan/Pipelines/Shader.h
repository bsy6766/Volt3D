/**
*	@file Shader.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_SHADER_H
#define V3D_VK_SHADER_H

#include <vulkan/vulkan.hpp>

#include <filesystem>

#include <glslang/Public/ShaderLang.h>

#include "utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class VOLT3D_DLL Shader
{
private:
	const vk::Device& logicalDevice;
	std::filesystem::path filePath;
	vk::ShaderStageFlagBits stage;
	vk::ShaderModule shaderModule;

	EShLanguage getEShLanguage() const;

	std::vector<char> readFile( const std::filesystem::path& filePath );

public:
	Shader() = delete;
	Shader(const std::filesystem::path& filePath);
	~Shader();
	
	bool init( const std::filesystem::path& filePath );

	/** Get Vulkan ShaderModule */
	const vk::ShaderModule get() const;

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