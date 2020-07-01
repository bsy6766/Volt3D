/**
*	@file AssetLoader.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "AssetLoader.h"

#include "Shader/Shader.h"
#include "Shader/ShaderCache.h"
#include "Shader/ShaderLoader.h"
#include "Texture/Texture.h"
#include "Texture/Texture2D.h"
#include "Texture/TextureCache.h"
#include "Texture/TextureLoader.h"


V3D_NS_BEGIN

AssetLoader::AssetLoader() {}

bool AssetLoader::init()
{
	// -- Register possible assets ------------
	supportedLoaderType.emplace( std::pair<std::type_index, std::type_index>( std::type_index( typeid(v3d::Texture) ), std::type_index( typeid(v3d::TextureLoader) ) ) );
	supportedLoaderType.emplace( std::pair<std::type_index, std::type_index>( std::type_index( typeid(v3d::Texture2D) ), std::type_index( typeid(v3d::TextureLoader) ) ) );

	supportedLoaderType.emplace( std::pair<std::type_index, std::type_index>( std::type_index( typeid(v3d::Shader) ), std::type_index( typeid(v3d::ShaderLoader) ) ) );
	// ----------------------------------------

	// -- Add loaders --------------------------
	loaders.emplace( std::pair< std::type_index, std::unique_ptr<v3d::BaseLoader>>( std::type_index( typeid(v3d::TextureLoader) ), std::move( std::unique_ptr<v3d::TextureLoader>( new v3d::TextureLoader() ) ) ) );
	loaders.emplace( std::pair< std::type_index, std::unique_ptr<v3d::BaseLoader>>( std::type_index( typeid(v3d::ShaderLoader) ), std::move( std::unique_ptr<v3d::ShaderLoader>( new v3d::ShaderLoader() ) ) ) );
	// ----------------------------------------

	return true;
}

AssetLoader::~AssetLoader() {}

std::optional<std::type_index> AssetLoader::getLoaderIndex( const std::type_info& typeInfo )
{
	if (auto find_it = supportedLoaderType.find( typeInfo ); find_it != supportedLoaderType.end())
		return find_it->second;
	else
		return std::nullopt;
}

std::shared_ptr<v3d::BaseAsset> AssetLoader::loadAsset( const std::type_index loaderIndex, const std::string& name, const std::filesystem::path& filePath )
{
	if (auto find_it = loaders.find( loaderIndex ); find_it != loaders.end())
		if (v3d::BaseAsset* newAsset = (find_it->second)->load( name, filePath ); newAsset != nullptr)
			return std::move( std::shared_ptr<v3d::BaseAsset>( newAsset ) );

	return nullptr;
}

V3D_NS_END