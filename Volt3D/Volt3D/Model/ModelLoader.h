/**
*	@file ModelLoader.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_MODEL_LOADER_H
#define V3D_MODEL_LOADER_H

#include "Utils/Macros.h"

V3D_NS_BEGIN

class ModelData;

class ModelLoader
{
private:
	ModelLoader();
	
public:
	~ModelLoader();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( ModelLoader );
	DEFAULT_MOVE_CONSTRUCTORS( ModelLoader );

	v3d::ModelData* load(const std::filesystem::path& modelFilePath );
};

V3D_NS_END

#endif