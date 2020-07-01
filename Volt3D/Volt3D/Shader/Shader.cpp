/**
*	@file Shader.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Shader.h"

#include "ShaderState.h"
#include "Vulkan/Pipelines/ShaderModule.h"
#include "Vulkan/Devices/LogicalDevice.h"

V3D_NS_BEGIN

Shader::Shader( const std::string& name, const std::filesystem::path& shaderFilePath )
	: v3d::BaseAsset( name )
	, filePath( shaderFilePath )
	, shaderModule( nullptr )
{}

Shader::~Shader()
{
	SAFE_DELETE( shaderModule );
	SAFE_DELETE( shaderState );
}

Shader* Shader::create( const std::string & name, const std::filesystem::path & shaderFilePath )
{
	if (shaderFilePath.empty())
	{
		// @todo Empty File Path Error
		return nullptr;
	}

	v3d::Shader* newShader = new (std::nothrow) v3d::Shader( name, shaderFilePath );
	if (newShader)
	{
		if (newShader->compile())
		{
			return newShader;
		}

		SAFE_DELETE( newShader );
	}

	return nullptr;
}

bool Shader::compile()
{
	// 0. Check path
	if (filePath.empty())
		return false;

	// 1. Create shader module and state
	shaderModule = new (std::nothrow) v3d::vulkan::ShaderModule();
	if (shaderModule == nullptr)
	{
		return false;
	}
	shaderState = new (std::nothrow) v3d::ShaderState();
	if (shaderState == nullptr)
	{
		return false;
	}

	// 2. Create vulkan shader module...
	std::vector<char> sourceCode;

	std::ifstream file( filePath, std::ios::ate | std::ios::binary );
	if (!file.is_open())
	{
		v3d::Logger::getInstance().error( "Failed to read shader file at \"{}\"", filePath.string().c_str() );
		return false;
	}

	std::size_t fileSize = (std::size_t)file.tellg();
	sourceCode.resize( fileSize );

	file.seekg( 0 );
	file.read( sourceCode.data(), fileSize );
	file.close();

	// 3. Compile
	if (!shaderModule->compile( sourceCode, filePath, *shaderState))
	{
		return false;
	}

	// 4. Assign id
	//id = v3d::Shader::idCounter++;

	// Done.
	return true;
}

inline v3d::ShaderState* Shader::getShaderState() const
{
	return shaderState;
}

inline const v3d::vulkan::ShaderModule& Shader::getShaderModule() const
{
	return *shaderModule;
}

std::vector<vk::DescriptorSetLayoutBinding> Shader::getDescriptorSetLayoutBinding() const
{
	std::vector<vk::DescriptorSetLayoutBinding> bindings;
	if (!shaderState) return bindings;

	shaderState->getDescriptorSetLayoutBinding( bindings );
	const auto stage = shaderModule->getStage();

	for (auto& binding : bindings)
	{
		binding.setStageFlags( stage );
	}

	return bindings;
}

void Shader::log() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.trace( "[Shader] info" );
	v3d::BaseAsset::log();
	logger.trace( "Stage: {}", vk::to_string(shaderModule->getStage()) );
}

V3D_NS_END