/**
*	@file ShaderLoader.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_SHADER_LOADER_H
#define V3D_SHADER_LOADER_H

#include <memory>
#include <string>
#include <filesystem>

#include "Resource/BaseLoader.h"

#include "Utils/Macros.h"

V3D_NS_BEGIN

class BaseAsset;

class VOLT3D_DLL ShaderLoader : public v3d::BaseLoader
{
	friend class AssetLoader;

private:
	ShaderLoader();

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( ShaderLoader );
	DEFAULT_MOVE_CONSTRUCTORS( ShaderLoader );

	~ShaderLoader();

	v3d::BaseAsset* load( const std::string& name, const std::filesystem::path& filePath ) override;
};

V3D_NS_END

#endif