/**
*	@file Shader.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "UniformBlock.h"

V3D_NS_BEGIN
VK_NS_BEGIN

UniformBlock::UniformBlock( const std::string& name, const int32_t binding, const int32_t size, const v3d::vulkan::UniformBlockType type, const bool writeOnly )
	: name( name )
	, binding( binding )
	, size( size )
	, type( type )
{}

UniformBlock::~UniformBlock() { uniforms.clear(); }

bool UniformBlock::hasUniform( const std::string& name ) const
{
	return uniforms.find( name ) != uniforms.end();
}

std::string UniformBlock::getName() const { return name; }

int32_t UniformBlock::getBinding() const { return binding; }

int32_t UniformBlock::getSize() const { return size; }

std::size_t UniformBlock::getCount() const { return uniforms.size(); }

v3d::vulkan::UniformBlockType UniformBlock::getType() const { return type; }

void UniformBlock::print( const bool detail )
{
	auto& logger = v3d::Logger::getInstance();

	logger.trace( "[UniformBlock] info..." );
	logger.trace( "Name: {}", name );
	logger.trace( "Type: {}", v3d::vulkan::uniformBlockTypeToString( type ) );
	logger.trace( "Binding: {}", binding );
	logger.trace( "Size: {}", size );

	if (detail)	for (auto& e : uniforms) (e.second).print();
}


VK_NS_END
V3D_NS_END