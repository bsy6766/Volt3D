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
	std::unordered_map<std::size_t, std::shared_ptr<v3d::Shader>> shaders;

public:
	// Destructor
	~ShaderCache();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( ShaderCache );
	DEFAULT_MOVE_CONSTRUCTORS( ShaderCache );

	static v3d::ShaderCache& get();

	bool hasShader( const std::size_t id ) const;
	bool hasShader( const std::string_view name ) const;
	bool hasShader( const std::shared_ptr<v3d::Shader>& shader ) const;

	bool addShader( const std::shared_ptr<v3d::Shader>& shader );

	//bool removeTexture( const std::size_t id );
	//bool removeTexture( const std::string_view name );
	//std::size_t removeAllTextures( const std::string_view name );
	//bool removeTexture( const std::shared_ptr<v3d::Texture>& texture );

	std::shared_ptr<v3d::Shader> getShader( const std::size_t id ) const;
	std::shared_ptr<v3d::Shader> getShader( const std::string_view name ) const;
	std::vector<std::shared_ptr<v3d::Shader>> getAllShaders( const std::string_view name ) const;

	//std::size_t purge();

	/** Get total count of shaders in ShaderCache */
	inline std::size_t count() const;

	/** Log TextureCache */
	void log() const;

	void clear();
};

V3D_NS_END

#endif