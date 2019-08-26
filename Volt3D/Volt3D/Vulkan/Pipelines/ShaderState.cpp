/**
*	@file ShaderState.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "ShaderState.h"
#include "Uniform.h"
#include "UniformBlock.h"
#include "Attribute.h"

#include <SPIRV/GlslangToSpv.h>

V3D_NS_BEGIN
VK_NS_BEGIN

ShaderState::ShaderState()
	: attributes()
	, uniformBlocks()
	, uniforms()
{}

ShaderState::~ShaderState() 
{
	for (auto& attribute : attributes) SAFE_DELETE( attribute.second );
	for (auto& uniformBlock : uniformBlocks) SAFE_DELETE( uniformBlock.second );
	for (auto& uniform : uniforms) SAFE_DELETE( uniform.second );
	attributes.clear();
	uniformBlocks.clear();
	uniforms.clear();
}

void ShaderState::init( glslang::TProgram& program )
{
	// uniform blocks
	for (int i = 0; i < program.getNumLiveUniformBlocks(); i++)
	{
		const glslang::TObjectReflection& reflection = program.getUniformBlock( i );

		v3d::vulkan::UniformBlockType type = v3d::vulkan::UniformBlockType::eUndefined;
		if (reflection.getType()->getQualifier().storage == glslang::EvqUniform) type = v3d::vulkan::UniformBlockType::eUniform;
		else if (reflection.getType()->getQualifier().storage == glslang::EvqBuffer) type = v3d::vulkan::UniformBlockType::eStorage;
		else if (reflection.getType()->getQualifier().layoutPushConstant) type = v3d::vulkan::UniformBlockType::ePush;

		uniformBlocks.emplace( reflection.getBinding(), new v3d::vulkan::UniformBlock( reflection.name, reflection.getBinding(), reflection.size, type, false ) );
	}

	// uniforms
	for (int32_t i = 0; i < program.getNumLiveUniformVariables(); i++)
	{
		const glslang::TObjectReflection& reflection = program.getUniform( i );

		if (reflection.getBinding() == -1)
		{
			std::vector<std::string> split;
			std::stringstream ss( reflection.name );

			while (ss.good())
			{
				std::string substr;
				std::getline( ss, substr, '.' );
				split.push_back( substr );
			}

			if (split.size() > 1)
			{
				auto uniformBlock = getUniformBlock( split.front() );
				if (uniformBlock)
				{
					uniformBlock->uniforms.emplace( reflection.name, new v3d::vulkan::Uniform( reflection.name, reflection.getBinding(), reflection.offset, reflection.size, reflection.glDefineType, false ) );
				}
			}
		}
		else
		{
			// Uniforms
			auto& qualifier = reflection.getType()->getQualifier();
			reflection.dump();
			uniforms.emplace( reflection.getBinding(), new v3d::vulkan::Uniform( reflection.name, reflection.getBinding(), reflection.offset, reflection.size, reflection.glDefineType, qualifier.writeonly ) );
		}
	}

	// vertex attribs
	for (int32_t i{}; i < program.getNumLiveAttributes(); i++)
	{
		auto reflection = program.getPipeInput( i );

		if (reflection.name.empty()) break;
		auto& q = reflection.getType()->getQualifier();

		attributes.emplace( q.layoutLocation, new v3d::vulkan::Attribute( reflection.name, q.layoutSet, q.layoutLocation, 1, reflection.glDefineType ) );
		//auto t = reflection.getType();
	}

	for (auto& attribute : attributes) attribute.second->print();
	for (auto& uniformBlock : uniformBlocks) uniformBlock.second->print( true );
	for (auto& uniform : uniforms) uniform.second->print();
}

v3d::vulkan::Attribute* ShaderState::getAttribute( const uint32_t location ) const
{
	auto find_it = attributes.find( location );
	if (find_it == attributes.end()) return nullptr;
	return find_it->second;
}

v3d::vulkan::Attribute* ShaderState::getAttribute( const std::string_view name ) const
{
	for (auto& attribute : attributes) if ((attribute.second)->name == name) return attribute.second;
	return nullptr;
}

v3d::vulkan::UniformBlock* ShaderState::getUniformBlock( const uint32_t binding ) const
{
	auto find_it = uniformBlocks.find( binding );
	if (find_it == uniformBlocks.end()) return nullptr;
	return find_it->second;
}

v3d::vulkan::UniformBlock* ShaderState::getUniformBlock( const std::string_view name ) const
{
	for (auto& uniformBlock : uniformBlocks) if ((uniformBlock.second)->name == name) return uniformBlock.second;
	return nullptr;
}

v3d::vulkan::Uniform* ShaderState::getUniform( const uint32_t binding ) const
{
	auto find_it = uniforms.find( binding );
	if (find_it == uniforms.end()) return nullptr;
	return find_it->second;
}

v3d::vulkan::Uniform* ShaderState::getUniform( const std::string_view name ) const
{
	for (auto& uniform : uniforms) if ((uniform.second)->name == name) return uniform.second;
	return nullptr;
}


VK_NS_END
V3D_NS_END