/**
*	@file AssetCache.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "AssetCache.h"

#include "Shader/Shader.h"
#include "Shader/ShaderCache.h"
#include "Texture/Texture.h"
#include "Texture/Texture2D.h"
#include "Texture/TextureCache.h"

V3D_NS_BEGIN

AssetCache::AssetCache()
{}

bool AssetCache::init()
{
	// -- Register possible assets ------------
	supportedAssetTypes.emplace( std::pair<std::type_index, std::type_index>( std::type_index( typeid(v3d::Texture) ), std::type_index( typeid(v3d::TextureCache) ) ) );
	supportedAssetTypes.emplace( std::pair<std::type_index, std::type_index>( std::type_index( typeid(v3d::Texture2D) ), std::type_index( typeid(v3d::TextureCache) ) ) );
	
	supportedAssetTypes.emplace( std::pair<std::type_index, std::type_index>( std::type_index( typeid(v3d::Shader) ), std::type_index( typeid(v3d::ShaderCache) ) ) );
	// ----------------------------------------

	// -- Add caches --------------------------
	caches.emplace( std::pair< std::type_index, std::unique_ptr<v3d::BaseCache>>( std::type_index( typeid(v3d::TextureCache) ), std::move( std::unique_ptr<v3d::TextureCache>( new v3d::TextureCache() ) ) ) );
	caches.emplace( std::pair< std::type_index, std::unique_ptr<v3d::BaseCache>>( std::type_index( typeid(v3d::ShaderCache) ), std::move( std::unique_ptr<v3d::ShaderCache>( new v3d::ShaderCache() ) ) ) );
	// ----------------------------------------
	
	return true;
}

std::optional<std::type_index> AssetCache::getCacheIndex( const std::type_info& typeInfo )
{
	if (auto find_it = supportedAssetTypes.find( typeInfo ); find_it != supportedAssetTypes.end())
		return find_it->second;
	else
		return std::nullopt;
}

std::shared_ptr<v3d::BaseAsset> AssetCache::getAsset( const std::type_index cacheIndex, std::size_t id )
{
	if (const auto find_it = caches.find( cacheIndex ); find_it != caches.end())
		return (find_it->second)->get( id );
	return nullptr;
}

std::shared_ptr<v3d::BaseAsset> AssetCache::getAsset( const std::type_index cacheIndex, const std::string& name )
{
	if (const auto find_it = caches.find( cacheIndex ); find_it != caches.end())
		return (find_it->second)->get( name );
	return nullptr;
}

AssetCache::~AssetCache()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info( "[AssetCache] Destroyed" );
#endif

	clearAll();
}

void AssetCache::clearAll()
{
	for (auto& cache : caches)
	{
		(cache.second)->clearAll();
	}
}

V3D_NS_END

