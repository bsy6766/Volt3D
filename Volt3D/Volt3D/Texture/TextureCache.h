/**
*	@file TextureCache.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_TEXTURE_MANAGER_H
#define V3D_TEXTURE_MANAGER_H

#include "Utils/Macros.h"

#include "Texture.h"

V3D_NS_BEGIN

class Texture;

/**
*	@class TextureCache
*	@brief Manages all textures.
*/
class VOLT3D_DLL TextureCache
{
	friend class Engine;

private:
	// default constructor
	TextureCache();

	// id : texture
	std::unordered_map<std::size_t, std::shared_ptr<v3d::Texture>> textures;
		
public:
	// Destructor
	~TextureCache();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( TextureCache );
	DEFAULT_MOVE_CONSTRUCTORS( TextureCache );

	static v3d::TextureCache& get();

	bool hasTexture(const std::size_t id) const;
	bool hasTexture( const std::string_view name ) const;
	bool hasTexture( const std::shared_ptr<v3d::Texture>& texture ) const;

	bool addTexture( const std::shared_ptr<v3d::Texture>& texture );

	bool removeTexture( const std::size_t id );
	bool removeTexture( const std::string_view name );
	std::size_t removeAllTextures( const std::string_view name );
	bool removeTexture( const std::shared_ptr<v3d::Texture>& texture );

	std::shared_ptr<v3d::Texture> getTexture( const std::size_t id ) const;
	std::shared_ptr<v3d::Texture> getTexture( const std::string_view name ) const;
	std::vector<std::shared_ptr<v3d::Texture>> getAllTextures( const std::string_view name ) const;

	std::size_t purge();

	/** Log TextureCache */
	void log() const;

	void clear();
};

V3D_NS_END

#endif