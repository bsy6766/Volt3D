/**
*	@file TextureManager.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "TextureManager.h"

#include "Utils/Logger.h"
#include "Utils/FileSystem.h"
#include "Config/BuildConfig.h"

v3d::TextureManager::TextureManager( const uint32_t maxTextureSize )
	: maxTextureSize( maxTextureSize )
{}

v3d::TextureManager::~TextureManager()
{
	clear();
}

uint32_t v3d::TextureManager::getMaxTextureSize() const
{
	return maxTextureSize;
}

/*
bool v3d::TextureManager::hasTexture(const std::size_t id) const
{
	return textures.find(id) != textures.end();
}

bool v3d::TextureManager::hasTexture(const std::string & filePath) const
{
	// iterate
	for (auto& e : textures)
	{
		// check
		if (e.second)
		{
			// check name
			if ((e.second)->getFilePath() == filePath)
			{
				// Done.
				return true;
			}
		}
	}

	// Failed
	return false;
}

bool v3d::TextureManager::addTexture(std::shared_ptr<v3d::Texture2D> texture)
{
	// check nullptr
	if (texture == nullptr)
	{
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
		v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::TEXTURE_MANAGER, v3d::Error::Code::TMErrCode::CAN_NOT_ADD_NULLPTR_TEXTURE));
#endif

		// Failed
		return false;
	}

	// Check
	if (hasTexture(texture->getID()))
	{
		// Failed
		return false;
	}

	// Add
	textures.emplace(std::make_pair(texture->getID(), texture));

	// Done.
	return true;
}

bool v3d::TextureManager::removeTexture(const std::size_t id)
{
	// Find
	auto find_it = textures.find(id);

	// Check
	if (find_it == textures.end())
	{
		// Doesn't exists.
		return false;
	}
	else
	{
		// remove.
		textures.erase(find_it);

		//  Done.
		return true;
	}
}

bool v3d::TextureManager::removeTexture(const std::string & filePath)
{
	// file path can be empty

	// get iter
	auto iter = textures.begin();

	// iterate
	for (; iter != textures.end();)
	{
		// Check
		if ((*iter).second && (((*iter).second)->getFilePath() == filePath))
		{
			// matches. 

			// Remove
			textures.erase(iter);

			// Done.
			return true;
		}
	}

	// Failed
	return false;
}

std::shared_ptr<v3d::Texture2D> v3d::TextureManager::getTexture(const std::size_t id) const
{
	// find
	auto find_it = textures.find(id);

	// check
	if (find_it == textures.end())
	{
		// Failed
		return nullptr;
	}
	else
	{
		// Done.
		return (find_it->second);
	}
}

std::shared_ptr<v3d::Texture2D> v3d::TextureManager::getTexture(const std::string & filePath) const
{
	// File path can be empty

	// Iterate and compare path
	for (auto& e : textures)
	{
		// Check path
		if ((e.second)->getFilePath() == filePath)
		{
			// Done.
			return (e.second);
		}
	}

	// Failed
	return nullptr;
}

*/
void v3d::TextureManager::print() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.trace("[TextureManager] Info");
	logger.trace("Total textures: " + std::to_string(textures.size()));

	//int index = 0;

	//for (auto& e : textures)
	//{
	//	if (e.second)
	//	{
	//		logger.info("#" + std::to_string(index) + ") ID: " + std::to_string(e.first) + ", Ref Count: " + std::to_string((e.second).use_count()));

	//		(e.second)->print();

	//		index++;
	//	}
	//}
}

void v3d::TextureManager::clear()
{
	textures.clear();
}
