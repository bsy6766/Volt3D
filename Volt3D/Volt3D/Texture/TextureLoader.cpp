/**
*	@file TextureLoader.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "TextureLoader.h"

V3D_NS_BEGIN

TextureLoader::TextureLoader()
	: v3d::BaseLoader()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info( "[TextureLoader] Created" );
#endif
}

TextureLoader::~TextureLoader()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info( "[TextureLoader] Destroyed" );
#endif
}

v3d::BaseAsset* TextureLoader::load( const std::string& name, const std::filesystem::path& filePath )
{
	return nullptr;
}

V3D_NS_END