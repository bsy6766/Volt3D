/**
*	@file ShaderCache.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_SHADER_MANAGER_H
#define V3D_SHADER_MANAGER_H

#include "Utils/Macros.h"

V3D_NS_BEGIN

class Shader;

/**
*	@class TextureCache
*	@brief Manages all textures.
*/
class VOLT3D_DLL ShaderCache
{
	friend class Engine;

private:
	// default constructor
	ShaderCache();

	// id : shaders
	std::unordered_map<std::size_t, std::shared_ptr<v3d::Shader>> textures;

public:
	// Destructor
	~ShaderCache();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( ShaderCache );
	DEFAULT_MOVE_CONSTRUCTORS( ShaderCache );

	static v3d::ShaderCache& get();

	//bool hasTexture( const std::size_t id ) const;
	//bool hasTexture( const std::string_view name ) const;
	//bool hasTexture( const std::shared_ptr<v3d::Texture>& texture ) const;

	//bool addTexture( const std::shared_ptr<v3d::Texture>& texture );

	//bool removeTexture( const std::size_t id );
	//bool removeTexture( const std::string_view name );
	//std::size_t removeAllTextures( const std::string_view name );
	//bool removeTexture( const std::shared_ptr<v3d::Texture>& texture );

	//std::shared_ptr<v3d::Texture> getTexture( const std::size_t id ) const;
	//std::shared_ptr<v3d::Texture> getTexture( const std::string_view name ) const;
	//std::vector<std::shared_ptr<v3d::Texture>> getAllTextures( const std::string_view name ) const;

	//std::size_t purge();

	///** Log TextureCache */
	//void log() const;

	void clear();
};

V3D_NS_END

#endif