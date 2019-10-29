/**
*	@file ShaderCache.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "ShaderCache.h"

#include "Shader.h"
#include "Engine/Engine.h"
#include "Utils/Logger.h"
#include "Utils/FileSystem.h"
#include "Config/BuildConfig.h"

V3D_NS_BEGIN

ShaderCache::ShaderCache()
	: shaders()
{}

ShaderCache::~ShaderCache()
{
	clear();
}

v3d::ShaderCache& ShaderCache::get()
{
	return v3d::Engine::get()->getShaderCache();
}

bool ShaderCache::hasShader( const std::size_t id ) const
{
	return shaders.find( id ) != shaders.end();
}

bool ShaderCache::hasShader( const std::string_view name ) const
{
	for (auto& shader : shaders)
		if (shader.second && shader.second->getName() == name)
			return true;
	return false;
}

bool ShaderCache::hasShader( const std::shared_ptr<v3d::Shader>& shader ) const
{
	if (shader == nullptr)
		return false;
	return hasShader( shader->getID() ) ? true : hasShader( shader->getName() ) ? true : false;
}

bool ShaderCache::addShader( const std::shared_ptr<v3d::Shader>& shader )
{
	if (!shader)
		return false;
	if (hasShader( shader->getID() ))
	{
		v3d::Logger::getInstance().warn( "[ShaderCache] already has a [Shader] with id: {}", shader->getID() );
		return false;
	}
	shaders.emplace( shader->getID(), shader );
	
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().debug( "[ShaderCache] Added [Shader] ID: {}, Name: {}", shader->getID(), shader->getName() );
#endif
	
	return true;
}

//bool ShaderCache::removeTexture( const std::size_t id )
//{
//	// 0 id does not exist
//	if (id == 0) return false;
//	auto find_it = textures.find( id );
//	if (find_it == textures.end()) return false;
//	textures.erase( find_it );
//	return true;
//}
//
//bool ShaderCache::removeTexture( const std::string_view name )
//{
//	for (auto iter = textures.begin(); iter != textures.end(); )
//	{
//		if ((*iter).second && ((*iter).second)->getName() == name)
//		{
//			textures.erase( iter );
//			return true;
//		}
//		iter++;
//	}
//	return false;
//}
//
//std::size_t ShaderCache::removeAllTextures( const std::string_view name )
//{
//	std::size_t c = 0;
//	for (auto iter = textures.begin(); iter != textures.end(); )
//	{
//		if ((*iter).second && ((*iter).second)->getName() == name)
//		{
//			iter = textures.erase( iter );
//			c++;
//			continue;
//		}
//		iter++;
//	}
//	return c;
//}
//
//bool ShaderCache::removeTexture( const std::shared_ptr<v3d::Texture>& texture )
//{
//	if (texture == nullptr) return false;
//	return removeTexture( texture->getID() );
//}
//
std::shared_ptr<v3d::Shader> ShaderCache::getShader( const std::size_t id ) const
{
	if (id == 0) return nullptr;
	return shaders.find( id )->second;
}

std::shared_ptr<v3d::Shader> ShaderCache::getShader( const std::string_view name ) const
{
	for (auto& shader : shaders)
		if ((shader.second)->getName() == name)
			return shader.second;
	return nullptr;
}

std::vector<std::shared_ptr<v3d::Shader>> ShaderCache::getAllShaders( const std::string_view name ) const
{
	std::vector<std::shared_ptr<v3d::Shader>> ret;

	for (auto& shader : shaders)
		if ((shader.second)->getName() == name)
			ret.push_back( shader.second );
	return ret;
}

//std::size_t ShaderCache::purge()
//{
//	std::size_t c = 0;
//	for (auto iter = textures.begin(); iter != textures.end();)
//	{
//		if (iter->second == nullptr || iter->second.use_count() == 1)
//		{
//			iter = textures.erase( iter );
//			c++;
//			continue;
//		}
//		iter++;
//	}
//	return c;
//}

inline std::size_t ShaderCache::count() const
{
	return shaders.size();
}

void ShaderCache::log() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.trace( "[ShaderCahce] Info" );
	logger.trace( "Total shaders: " + std::to_string( shaders.size() ) );

	std::size_t i = 0;
	for (auto& shader : shaders) if (shader.second) shader.second->log();
}

void ShaderCache::clear()
{
	shaders.clear();
}

V3D_NS_END