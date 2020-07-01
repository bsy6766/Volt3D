/**
*	@file TextureCache.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_TEXTURE_MANAGER_H
#define V3D_TEXTURE_MANAGER_H

#include "Utils/Macros.h"

#include "Resource/BaseCache.h"

#include "Texture.h"

V3D_NS_BEGIN

class Texture;

/**
*	@class TextureCache
*	@brief Manages all textures.
*/
class VOLT3D_DLL TextureCache : public v3d::BaseCache
{
	friend class Engine;
	friend class ResourceManager;

private:
	// default constructor
	TextureCache();

	static std::size_t idCounter;

	virtual bool isSupported( std::type_info& typeInfo ) override;
public:

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( TextureCache );
	DEFAULT_MOVE_CONSTRUCTORS( TextureCache );

	// Destructor
	~TextureCache();

	//static v3d::TextureCache& get();

	virtual void log() const override;
};

V3D_NS_END

#endif