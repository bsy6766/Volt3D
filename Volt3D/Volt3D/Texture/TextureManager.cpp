/**
*	@file TextureManager.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "TextureManager.h"

#include "Texture2D.h"
#include "Engine/Engine.h"
#include "Utils/Logger.h"
#include "Utils/FileSystem.h"
#include "Config/BuildConfig.h"

V3D_NS_BEGIN

TextureManager::TextureManager()
	: textures()
{}

TextureManager::~TextureManager()
{
	clear();
}

v3d::TextureManager& TextureManager::get()
{
	return v3d::Engine::get()->getTextureManager();
}

bool TextureManager::hasTexture( const std::size_t id ) const
{
	return textures.find( id ) != textures.end();
}

bool TextureManager::hasTexture( const std::string_view name ) const
{
	for (auto& texture : textures)
		if (texture.second && texture.second->getName() == name) 
			return true;
	return false;
}

bool TextureManager::hasTexture( const std::shared_ptr<v3d::Texture>& texture ) const
{
	if (texture == nullptr) 
		return false;
	return hasTexture( texture->getID() ) ? true : hasTexture( texture->getName() ) ? true : false;
}

bool TextureManager::addTexture( const std::shared_ptr<v3d::Texture>& texture )
{
	if (!texture) 
		return false;
	if (hasTexture( texture->getID() ))
	{
		v3d::Logger::getInstance().warn( "[TextureManager] already has a [Texture] with id: {}", texture->getID() );
		return false;
	}
	textures.emplace( texture->getID(), texture );
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().debug( "[TextureManager] Added [Texture] ID: {}, Name: {}", texture->getID(), texture->getName() );
#endif
	return true;
}

bool TextureManager::removeTexture( const std::size_t id )
{
	// 0 id does not exist
	if (id == 0) return false;
	auto find_it = textures.find( id );
	if (find_it == textures.end()) return false;
	textures.erase( find_it );
	return true;
}

bool TextureManager::removeTexture( const std::string_view name )
{
	for (auto iter = textures.begin(); iter != textures.end(); )
	{
		if ((*iter).second && ((*iter).second)->getName() == name)
		{
			textures.erase( iter );
			return true;
		}
		iter++;
	}
	return false;
}

std::size_t TextureManager::removeAllTextures( const std::string_view name )
{
	std::size_t c = 0;
	for (auto iter = textures.begin(); iter != textures.end(); )
	{
		if ((*iter).second && ((*iter).second)->getName() == name)
		{
			iter = textures.erase( iter );
			c++;
			continue;
		}
		iter++;
	}
	return c;
}

bool TextureManager::removeTexture( const std::shared_ptr<v3d::Texture>& texture )
{
	if (texture == nullptr) return false;
	return removeTexture( texture->getID() );
}

std::shared_ptr<v3d::Texture> TextureManager::getTexture( const std::size_t id ) const
{
	if (id == 0) return nullptr;
	return textures.find( id )->second;
}

std::shared_ptr<v3d::Texture> TextureManager::getTexture( const std::string_view name ) const
{
	for (auto& texture : textures)
		if ((texture.second)->getName() == name)
			return texture.second;
	return nullptr;
}

std::vector<std::shared_ptr<v3d::Texture>> TextureManager::getAllTextures( const std::string_view name ) const
{
	std::vector<std::shared_ptr<v3d::Texture>> ret;

	for (auto& texture : textures)
		if ((texture.second)->getName() == name)
			ret.push_back( texture.second );
	return ret;
}

std::size_t TextureManager::purge()
{
	std::size_t c = 0;
	for (auto iter = textures.begin(); iter != textures.end();)
	{
		if (iter->second == nullptr || iter->second.use_count() == 0)
		{
			iter = textures.erase( iter );
			c++;
			continue;
		}
		iter++;
	}
	return c;
}

void TextureManager::log() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.trace("[TextureManager] Info");
	logger.trace("Total textures: " + std::to_string(textures.size()));

	std::size_t i = 0;
	for (auto& texture : textures) if (texture.second) texture.second->log();
}

void TextureManager::clear()
{
	textures.clear();
}

V3D_NS_END