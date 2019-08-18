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

Attribute::Attribute( const int32_t set, const int32_t location, const int32_t size, const int32_t glType )
	: set( set )
	, location( location )
	, size( size )
	, glType( glType )
{}

Attribute::~Attribute() {}

int32_t Attribute::getSet() const { return set; }

int32_t Attribute::getLocation() const { return location; }

int32_t Attribute::getSize() const { return size; }

int32_t Attribute::getGlType() const { return glType; }

VK_NS_END
V3D_NS_END