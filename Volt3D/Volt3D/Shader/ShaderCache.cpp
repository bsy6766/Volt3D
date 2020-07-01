/**
*	@file ShaderCache.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "ShaderCache.h"

#include "Engine/Engine.h"
#include "Utils/Logger.h"
#include "Utils/FileSystem.h"
#include "Config/BuildConfig.h"
#include "Resource/Resource.h"
#include "Resource/AssetCache.h"

V3D_NS_BEGIN

std::size_t ShaderCache::idCounter = 0;

ShaderCache::ShaderCache()
	: v3d::BaseCache()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info("[ShaderCache] Created");
#endif
}

ShaderCache::~ShaderCache()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info( "[ShaderCache] Destroyed" );
#endif
}

bool ShaderCache::isSupported( std::type_info& typeInfo )
{
	return (typeInfo == typeid(v3d::Shader));
}

//v3d::ShaderCache& ShaderCache::get()
//{
//	return *v3d::Resource::getAssetCache().getCache<v3d::ShaderCache>();
//}

void ShaderCache::log() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.trace( "[ShaderCache] Info" );
	logger.trace( "[ShaderCache] Total textures: " + std::to_string( assets.size() ) );

	v3d::BaseCache::log();
}

V3D_NS_END