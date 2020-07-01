/**
*	@file Resource.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Resource.h"

V3D_NS_BEGIN

std::unique_ptr<v3d::AssetLoader> v3d::Resource::assetLoader = nullptr;
std::unique_ptr<v3d::AssetCache> v3d::Resource::assetCache = nullptr;

bool Resource::init()
{
	// init asset loader
	v3d::AssetLoader* newAL = new (std::nothrow) v3d::AssetLoader();
	if (newAL == nullptr)
		return false;
	v3d::Resource::assetLoader = std::unique_ptr<v3d::AssetLoader>( newAL );
	if (!v3d::Resource::assetLoader->init())
		return false;

	// Init asset cache
	v3d::AssetCache* newAC = new (std::nothrow) v3d::AssetCache();
	if (newAC == nullptr) 
		return false;
	v3d::Resource::assetCache = std::unique_ptr<v3d::AssetCache>( newAC );
	if (!v3d::Resource::assetCache->init()) 
		return false;


	return true;
}

void Resource::release()
{
	v3d::Resource::assetLoader.reset();
	v3d::Resource::assetCache.reset();
}

bool Resource::loadScene( const std::filesystem::path& filePath )
{
	if (filePath.empty()) return false;
	//if (v3d::Resource::assetLoader == nullptr || v3d::Resource::assetCache == nullptr) return false;

	//assetLoader->loadScene(filePath, v3d::Resource::assetCache.)

	return true;
}

void Resource::clearAll()
{
	if (v3d::Resource::assetCache)
		v3d::Resource::assetCache->clearAll();
}

v3d::AssetCache& Resource::getAssetCache()
{
	return *assetCache;
}

V3D_NS_END