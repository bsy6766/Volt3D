/**
*	@file ShaderLoader.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "ShaderLoader.h"

V3D_NS_BEGIN

ShaderLoader::ShaderLoader()
	: v3d::BaseLoader()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info( "[ShaderLoader] Created" );
#endif
}

ShaderLoader::~ShaderLoader()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info( "[ShaderLoader] Destroyed" );
#endif
}

v3d::BaseAsset* ShaderLoader::load( const std::string& name, const std::filesystem::path& filePath )
{
	return nullptr;
}

V3D_NS_END