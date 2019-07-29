/**
*	@file TextureManager.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_TEXTURE_MANAGER_H
#define V3D_TEXTURE_MANAGER_H

#include <string>
#include <memory>

#include "Utils/Macros.h"

namespace v3d
{
	// Foward declaration
	class Texture;

	/**
	*	@class TextureManager
	*	@brief Manages all textures.
	*/
	class VOLT3D_DLL TextureManager
	{
		friend class Engine;

	private:
		// default constructor
		TextureManager() = delete;
		TextureManager( const uint32_t maxTextureSize );

		// Max texture size 
		uint32_t maxTextureSize;

		// stores all texture
		std::unordered_map<std::size_t, std::shared_ptr<v3d::Texture>> textures;
		
	public:
		// Destructor
		~TextureManager();

		/**
		*	Get maximum texture size.
		*	This value is queried with GL_MAX_TEXTURE_SIZE.
		*	@return Maximum texture size.
		*/
		uint32_t getMaxTextureSize() const;

		///**
		//*	Check if there is a texture with same id.
		//*	@param id A texture id to query.
		//*	@return true if exists. Else, false.
		//*/
		//bool hasTexture(const std::size_t id) const;

		///**
		//*	Check if there is a texture with same texture file path that was created.
		//*	@param filePath A texture file path to query.
		//*	@return true if exists. Else, false.
		//*/
		//bool hasTexture(const std::string& filePath) const;

		///**
		//*	Add texture to texture manager.
		//*	@param texture A Texture2D instance to add.
		//*	@return true if successfully add texture. Else, false.
		//*/
		//bool addTexture(std::shared_ptr<v3d::Texture> texture);

		///**
		//*	Remove texture from texture manager.
		//*	@param id A texture id to remove.
		//*	@return true if successfully removes. Else, false.
		//*/
		//bool removeTexture(const std::size_t id);

		///**
		//*	Remove texture from texture manager with same file path
		//*	@warning This will remove the first texture that matches the file path.
		//*	@param filePath A texture name to remove.
		//*	@return true if successfully removes. Else, false.
		//*/
		//bool removeTexture(const std::string& filePath);

		///**
		//*	Get texture by id.
		//*	@param id Texture id to query.
		//*	@return A Texture instance if success. Else, nullptr.
		//*/
		//std::shared_ptr<v3d::Texture> getTexture(const std::size_t id) const;

		///**
		//*	Get texture by file path.
		//*	@param filePath A texture name to query.
		//*	@return A Texture instance if success. Else, nullptr.
		//*/
		//std::shared_ptr<v3d::Texture> getTexture(const std::string& filePath) const;

		// print all texture info
		void print() const;

		// release all textures
		void clear();
	};
}

#endif