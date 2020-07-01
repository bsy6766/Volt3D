/**
*	@file TextureCache.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "TextureCache.h"

#include "Texture2D.h"
#include "Engine/Engine.h"
#include "Utils/Logger.h"
#include "Utils/FileSystem.h"
#include "Config/BuildConfig.h"
#include "Resource/Resource.h"
#include "Resource/AssetCache.h"

V3D_NS_BEGIN

std::size_t TextureCache::idCounter = 0;

TextureCache::TextureCache()
	: v3d::BaseCache()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info("[TextureCache] Created");
#endif
}

TextureCache::~TextureCache()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info( "[TextureCache] Destroyed" );
#endif
}

bool TextureCache::isSupported( std::type_info& typeInfo )
{
	return (typeInfo == typeid(v3d::Texture)) || (typeInfo == typeid(v3d::Texture2D));
}

//v3d::TextureCache& TextureCache::get()
//{
//	auto ret = v3d::Resource::getAssetCache().getCache<v3d::TextureCache>();
//	if (ret.has_value())
//		return *ret.value();
//}

void TextureCache::log() const
{
	auto& logger = v3d::Logger::getInstance();
	
	logger.trace("[TextureCache] Info");
	logger.trace("[TextureCache] Total textures: " + std::to_string(assets.size()));
	
	v3d::BaseCache::log();
}

V3D_NS_END