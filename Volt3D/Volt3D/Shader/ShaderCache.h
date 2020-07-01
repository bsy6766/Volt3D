/**
*	@file ShaderCache.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_SHADER_MANAGER_H
#define V3D_SHADER_MANAGER_H

#include "Utils/Macros.h"

#include "Resource/BaseCache.h"

#include "Shader.h"

V3D_NS_BEGIN

class Shader;

/**
*	@class ShaderCache
*	@brief Manages all textures.
*/
class VOLT3D_DLL ShaderCache : public v3d::BaseCache
{
	friend class Engine;
	friend class AssetCache;
	friend class AssetLoader;
	friend class ResourceManager;

private:
	// default constructor
	ShaderCache();

	static std::size_t idCounter;

	virtual bool isSupported( std::type_info& typeInfo ) override;

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( ShaderCache );
	DEFAULT_MOVE_CONSTRUCTORS( ShaderCache );

	// Destructor
	~ShaderCache();

	//static v3d::ShaderCache& get();

	virtual void log() const override;
};

V3D_NS_END

#endif