/**
*	@file BaseAsset.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "BaseAsset.h"

#include <type_traits>

V3D_NS_BEGIN

BaseAsset::BaseAsset( const std::string& name )
	: name( name )
	, id( 0 )
{}

BaseAsset::~BaseAsset() {}

bool BaseAsset::assignID( const std::size_t id )
{
	// All asset id must be greater than 0. 0 means it's not managed by the engine
	if (id <= 0) return false;
	this->id = id;
	return true;
}

bool BaseAsset::isManaged() const
{
	return id != 0;
}

std::size_t BaseAsset::getID() const
{
	return id;
}

std::string BaseAsset::getName() const
{
	return name;
}

void BaseAsset::log() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.trace( "ID: {}", id );
	logger.trace( "Name: {}", name );
}

V3D_NS_END