/**
*	@file BaseLoader.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_BASE_LOADER_H
#define V3D_BASE_LOADER_H

#include <memory>
#include <string>
#include <filesystem>

#include "Utils/Macros.h"

V3D_NS_BEGIN

class BaseAsset;

class VOLT3D_DLL BaseLoader
{
protected:
	BaseLoader();

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( BaseLoader );
	DEFAULT_MOVE_CONSTRUCTORS( BaseLoader );

	virtual ~BaseLoader();

	virtual v3d::BaseAsset* load( const std::string& name, const std::filesystem::path& filePath ) = 0;
};

V3D_NS_END

#endif