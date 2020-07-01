/**
*	@file Resource.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_RESOURCE_H
#define V3D_RESOURCE_H

#include <filesystem>
#include <memory>
#include <string>

#include "BaseAsset.h"
#include "BaseCache.h"

#include "Resource/AssetLoader.h"
#include "Resource/AssetCache.h"

#include "Utils/Macros.h"

V3D_NS_BEGIN

class AssetLoader;
class AssetCache;

/**
*	@todo Document this
*	
*	@note
*	Purpose of this class is to manage Resource in the engine.
*	
*/
class VOLT3D_DLL Resource
{
	friend class Engine;

private:
	// Static class
	Resource() = delete;

	// Loader and cache
	static std::unique_ptr<v3d::AssetLoader> assetLoader;
	static std::unique_ptr<v3d::AssetCache> assetCache;

	template<class T> static inline bool isAsset() { if constexpr (std::is_base_of_v<v3d::BaseAsset, T>) return true; else return false; }

	static bool init();
	static void release();
	
public:
	~Resource() = delete;

	static bool loadScene( const std::filesystem::path& filePath );

	template<typename T, typename std::enable_if<std::is_base_of<v3d::BaseAsset, T>::value>::type * = nullptr>
	static std::shared_ptr<T> get( const std::size_t id )
	{
		// id must be greater than 0
		if (id == 0) return nullptr;
		// Type must be asset
		if (!v3d::Resource::isAsset<T>()) return nullptr;
		// must have asset cache
		if (!v3d::Resource::assetCache) return nullptr;

		// get by id
		return v3d::Resource::assetCache->get<T>( id );
	}
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
	template<class T>
	static std::shared_ptr<T> get( const std::string& name )
	{
		// Must have name
		if (name.empty()) return nullptr;
		// Type must be asset
		if (!v3d::Resource::isAsset<T>()) return nullptr;
		// must have asset cache
		if (!v3d::Resource::assetCache) return nullptr;

		// get by name
		return v3d::Resource::assetCache->get<T>( name );
	}

	template<class T>
	static std::shared_ptr<T> load( const std::string& name, const std::filesystem::path& filePath )
	{
		// Must have name and filePath
		if (name.empty() || filePath.empty() || !std::filesystem::is_regular_file(filePath)) return nullptr;
		// Type must be asset
		if (!v3d::Resource::isAsset<T>()) return nullptr;
		// Must have asset cache and assetLoader
		if (!v3d::Resource::assetCache || !v3d::Resource::assetLoader) return nullptr;

		// Check if AssetCache has it
		if (auto asset = v3d::Resource::assetCache->get<T>( name ); asset != nullptr)
		{
			// Already has asset
			return asset;
		}
		else
		{
			// Load
			return v3d::Resource::assetLoader->load<T>( name, filePath );
		}
	}

	template<class T>
	static void clear()
	{
		if (v3d::Resource::assetCache)
			v3d::Resource::assetCache->clear<T>();
	}

	static void clearAll();

	static v3d::AssetCache& getAssetCache();
};

V3D_NS_END

#endif