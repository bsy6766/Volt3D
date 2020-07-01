/**
*	@file TextureLoader.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_TEXTURE_LOADER_H
#define V3D_TEXTURE_LOADER_H

#include <memory>
#include <string>
#include <filesystem>

#include "Resource/BaseLoader.h"

#include "Utils/Macros.h"

V3D_NS_BEGIN

class BaseAsset;

class VOLT3D_DLL TextureLoader : public v3d::BaseLoader
{
	friend class AssetLoader;

private:
	TextureLoader();

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( TextureLoader );
	DEFAULT_MOVE_CONSTRUCTORS( TextureLoader );

	~TextureLoader();

	v3d::BaseAsset* load( const std::string& name, const std::filesystem::path& filePath ) override;
};

V3D_NS_END

#endif