/**
*	@file Attribute.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Attribute.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Attribute::Attribute( const std::string& name, const int32_t set, const int32_t location, const int32_t size, const int32_t glValueType )
	: name( name )
	, set( set )
	, location( location )
	, size( size )
	, glValueType( glValueType )
{}

Attribute::~Attribute() {}

std::string Attribute::getName() const { return name; }

int32_t Attribute::getSet() const { return set; }

int32_t Attribute::getLocation() const { return location; }

int32_t Attribute::getSize() const { return size; }

int32_t Attribute::getGlValueType() const { return glValueType; }

v3d::vulkan::GLType Attribute::getGLType() const { return v3d::vulkan::toGLTypeEnum( glValueType ); }

void Attribute::print() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.trace( "[Attribute] info..." );
	logger.trace( "Name: {}", name );
	logger.trace( "Set: {}", set );
	logger.trace( "Location: {}", location );
	logger.trace( "Size: {}", size );
	logger.trace( "GLType: {} ({})", glValueType, v3d::vulkan::GLDataTypeToString( getGLType() ) );
}

VK_NS_END
V3D_NS_END