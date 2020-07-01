/**
*	@file AssetLoader.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_ASSET_LOADER_H
#define V3D_ASSET_LOADER_H

#include <unordered_map>
#include <filesystem>
#include <typeinfo>
#include <typeindex>
#include <optional>
#include <memory>
#include <string>

#include "BaseLoader.h"

#include "Utils/Macros.h"

V3D_NS_BEGIN

class VOLT3D_DLL AssetLoader
{
	friend class Resource;
private:
	AssetLoader();

	std::unordered_map<std::type_index, std::type_index> supportedLoaderType;
	std::unordered_map<std::type_index, std::unique_ptr<v3d::BaseLoader>> loaders;

	bool init();

	std::optional<std::type_index> getLoaderIndex( const std::type_info& typeInfo );
	std::shared_ptr<v3d::BaseAsset> loadAsset( const std::type_index loaderIndex, const std::string& name, const std::filesystem::path& filePath );

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( AssetLoader );
	DEFAULT_MOVE_CONSTRUCTORS( AssetLoader );

	~AssetLoader();

	template<class T> 
	std::shared_ptr<T> load( const std::string& name, const std::filesystem::path& filePath )
	{
		if (std::optional<std::type_index> loaderIndex = getLoaderIndex( typeid(T) ); loaderIndex.has_value())
			if (std::shared_ptr<v3d::BaseAsset> asset = loadAsset( loaderIndex.value(), name, filePath ); asset != nullptr)
				return std::dynamic_pointer_cast<T>(asset);
		return nullptr;
	}
};

V3D_NS_END

#endif