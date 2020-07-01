/**
*	@file AssetCache.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_ASSET_CACHE_H
#define V3D_ASSET_CACHE_H

#include "Utils/Macros.h"

#include "BaseAsset.h"
#include "BaseCache.h"

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <optional>
#include <memory>

V3D_NS_BEGIN

class VOLT3D_DLL AssetCache
{
	friend class Resource;
	friend class TextureCache;
	friend class ShaderCache;

private:
	AssetCache();

	std::unordered_map<std::type_index, std::type_index> supportedAssetTypes;
	std::unordered_map<std::type_index, std::unique_ptr<v3d::BaseCache>> caches;

	bool init();

	template<class T> inline bool isCache() { if constexpr (std::is_base_of_v<v3d::BaseCache, T>) return true; else return false; }

	std::optional<std::type_index> getCacheIndex( const std::type_info& typeInfo );
	std::shared_ptr<v3d::BaseAsset> getAsset( const std::type_index cacheIndex, const std::size_t id );
	std::shared_ptr<v3d::BaseAsset> getAsset( const std::type_index cacheIndex, const std::string& name );
	
	template<class T>
	std::optional<T*> getCache()
	{
		if (isCache<T>())
		{
			auto find_it = caches.find( std::type_index( typeid(T) ) );
			if (find_it == caches.end())
				return std::nullopt;
			else
				return std::optional<T*>( (find_it->second).get() );
		}
	}

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( AssetCache );
	DEFAULT_MOVE_CONSTRUCTORS( AssetCache );

	~AssetCache();

	template<class T>
	std::shared_ptr<T> get( const std::size_t id )
	{
		if (std::optional<std::type_index> cacheIndex = getCacheIndex( typeid(T) ); cacheIndex.has_value())
			if (std::shared_ptr<v3d::BaseAsset> asset = getAsset( cacheIndex.value(), id ); asset != nullptr)
				return std::dynamic_pointer_cast<T>(asset);
		return nullptr;
	}

	template<class T>
	std::shared_ptr<T> get( const std::string& name )
	{
		if (std::optional<std::type_index> cacheIndex = getCacheIndex( typeid(T) ); cacheIndex.has_value())
			if (std::shared_ptr<v3d::BaseAsset> asset = getAsset( cacheIndex.value(), name ); asset != nullptr)
				return std::dynamic_pointer_cast<T>(asset);
		return nullptr; 
	}

	template<class T>
	std::shared_ptr<T> load( const std::filesystem::path& filePath )
	{
		return nullptr;
	}
	
	template<class T>
	void clear()
	{
	}

	void clearAll();
};

V3D_NS_END

#endif