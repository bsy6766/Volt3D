/**
*	@file ShaderState.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "ShaderState.h"

#include <SPIRV/GlslangToSpv.h>

V3D_NS_BEGIN
VK_NS_BEGIN

ShaderState::ShaderState()
	: attributes()
	, uniformBlocks()
	, uniforms()
{}

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

		uniformBlocks.emplace( reflection.getBinding(), std::move( v3d::vulkan::UniformBlock( reflection.name, reflection.getBinding(), reflection.size, type, false ) ) );
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
				if (uniformBlock.has_value())
				{
					uniformBlock.value().get().uniforms.emplace( reflection.name, std::move( v3d::vulkan::Uniform( reflection.name, reflection.getBinding(), reflection.offset, reflection.size, reflection.glDefineType, false ) ) );
				}
			}
		}
		else
		{
			// Uniforms
			auto& qualifier = reflection.getType()->getQualifier();
			reflection.dump();
			uniforms.emplace( reflection.getBinding(), std::move( v3d::vulkan::Uniform( reflection.name, reflection.getBinding(), reflection.offset, reflection.size, reflection.glDefineType, qualifier.writeonly ) ) );
		}
	}

	// vertex attribs
	for (int32_t i{}; i < program.getNumLiveAttributes(); i++)
	{
		auto reflection = program.getPipeInput( i );

		if (reflection.name.empty()) break;
		auto& q = reflection.getType()->getQualifier();

		v3d::vulkan::Attribute attribute( q.layoutSet, q.layoutLocation, 1, reflection.glDefineType );
		attributes.emplace( attribute.location, std::move( attribute ) );
		//auto t = reflection.getType();
	}

	for (auto& attribute : attributes) attribute.second.print();
	for (auto& uniformBlock : uniformBlocks) uniformBlock.second.print( true );
	for (auto& uniform : uniforms) uniform.second.print();
}

ShaderState::~ShaderState() {}

std::optional<std::reference_wrapper<v3d::vulkan::UniformBlock>> ShaderState::getUniformBlock( const uint32_t binding ) const
{
	auto find_it = uniformBlocks.find( binding );
	if (find_it == uniformBlocks.end()) return std::nullopt;
	return std::optional(find_it->second);
}

std::optional<std::reference_wrapper<v3d::vulkan::UniformBlock>> ShaderState::getUniformBlock( const std::string_view name ) const
{
	for (auto& uniformBlock : uniformBlocks) if ((uniformBlock.second).name == name) return uniformBlock.second;
	return std::nullopt;
}


VK_NS_END
V3D_NS_END