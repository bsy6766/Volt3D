/**
*	@file Shader.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_SHADER_H
#define V3D_SHADER_H

#include <vulkan/vulkan.hpp>

#include <filesystem>

#include <glslang/Public/ShaderLang.h>

#include "utils/Macros.h"

V3D_NS_BEGIN
class ShaderState;
VK_NS_BEGIN
class ShaderModule;
VK_NS_END

class VOLT3D_DLL Shader
{
	friend class Pipeline;

protected:
	static std::size_t idCounter;

private:
	Shader( const std::string& name, const std::filesystem::path& shaderFilePath );

	std::size_t id;
	std::string name;
	std::filesystem::path filePath;

	v3d::vulkan::ShaderModule* shaderModule;
	v3d::ShaderState* shaderState;

	bool compile();

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Shader );
	DEFAULT_MOVE_CONSTRUCTORS( Shader );

	~Shader();

	static Shader* create( const std::string& name, const std::filesystem::path& shaderFilePath );

	/** Get id of shader */
	inline std::size_t getID() const;

	/** Get name of shader */
	inline std::string getName() const;

	/** Get shader state of this shader */
	inline v3d::ShaderState* getShaderState() const;

	/** Get shader module */
	inline const v3d::vulkan::ShaderModule& getShaderModule() const;

	std::vector<vk::DescriptorSetLayoutBinding> getDescriptorSetLayoutBinding() const;

	void log() const;
};

V3D_NS_END

#endif