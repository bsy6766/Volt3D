/**
*	@file AssetMeta.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_ASSET_META_H
#define V3D_ASSET_META_H

#include <filesystem>
#include <string>
#include <typeindex>

#include "Utils/Macros.h"

V3D_NS_BEGIN

class VOLT3D_DLL AssetMeta
{
public:
	std::string name;
	std::filesystem::path path;
	std::type_index typeIndex;

	AssetMeta() = delete;
	AssetMeta( const std::string& name, const std::filesystem::path& path, const std::type_index& typeIndex )
		: name( name ), path( path ), typeIndex( typeIndex )
	{}
};

V3D_NS_END

#endif