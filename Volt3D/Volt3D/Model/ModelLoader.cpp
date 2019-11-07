/**
*	@file ModelLoader.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "ModelLoader.h"

#include "fx/gltf.h"

#include "ModelData.h"

V3D_NS_BEGIN

ModelLoader::ModelLoader()
{}

ModelLoader::~ModelLoader()
{}

v3d::ModelData* ModelLoader::load( const std::filesystem::path& modelFilePath )
{
	if (modelFilePath.empty()) return nullptr;
	if (!std::filesystem::exists( modelFilePath )) return nullptr;

	v3d::ModelData* model = new (std::nothrow) v3d::ModelData();
	if (!model) return nullptr;

	fx::gltf::Document modelDoc;

	try
	{
		modelDoc = fx::gltf::LoadFromText( modelFilePath.string() );
	}
	catch (std::system_error& s_err)
	{
		v3d::Logger::getInstance().error( "Failed to load model at \"{}\". Details: {}\n", modelFilePath.string(), s_err.what() );
		return nullptr;
	}

	const std::size_t totalTextures = modelDoc.textures.size();
	const std::size_t totalImages = modelDoc.images.size();
	const std::size_t totalSamplers = modelDoc.samplers.size();

	return model;
}


V3D_NS_END
