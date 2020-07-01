/**
*	@file BaseCache.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "BaseCache.h"

V3D_NS_BEGIN

BaseCache::BaseCache()
	: nameMap()
	, assets()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info( "[BaseCache] Created" );
#endif
}

BaseCache::~BaseCache()
{
	clear();
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info( "[BaseCache] Destroyed" );
#endif
}

/**
*	Check if this cache has asset with given id
*/
bool BaseCache::has( const std::size_t id ) const
{
	if (id == 0) return false;
	return assets.find( id ) != assets.end();
}

/**
*
*/
bool BaseCache::has( const std::string& name ) const
{
	if (name.empty()) return false;

	auto find_name_it = nameMap.find( name );
	if (find_name_it == nameMap.end())
		return false;

	auto find_it = assets.find( find_name_it->second );
	if (find_it == assets.end())
		return false;

	return true;
}

/**
*
*/
bool BaseCache::has( const std::shared_ptr<v3d::BaseAsset>& asset ) const
{
	if (asset == nullptr)
		return false;
	return has( asset->getID() ) ? true : has( asset->getName() ) ? true : false;
}

/**
*
*/
std::shared_ptr<v3d::BaseAsset> BaseCache::get( const std::size_t id ) const
{
	if (id == 0) return nullptr;
	auto find_it = assets.find( id );
	if (find_it == assets.end())
		return nullptr;
	else
		return find_it->second;
}

/**
*
*/
std::shared_ptr<v3d::BaseAsset> BaseCache::get( const std::string& name ) const
{
	if (name.empty()) return nullptr;

	auto find_name_it = nameMap.find( name );
	if (find_name_it == nameMap.end())
		return nullptr;

	auto find_it = assets.find( find_name_it->second );
	if (find_it == assets.end())
		return nullptr;

	return find_it->second;
}

/**
*
*/
std::vector<std::shared_ptr<v3d::BaseAsset>> BaseCache::getAll() const
{
	std::vector<std::shared_ptr<v3d::BaseAsset>> ret;

	for (auto& asset : assets)
		ret.push_back( asset.second );

	return ret;
}

/**
*
*/
bool BaseCache::remove( const std::size_t id )
{
	if (id == 0) return false;
	auto find_it = assets.find( id );
	if (find_it == assets.end()) return false;

	assets.erase( find_it );

	return true;
}

/**
*
*/
bool BaseCache::remove( const std::string& name )
{
	if (name.empty()) return false;

	auto find_name_it = nameMap.find( name );
	if (find_name_it == nameMap.end())
	{
		for (auto iter = assets.begin(); iter != assets.end();)
		{
			if ((iter->second)->getName() == name)
			{
				assets.erase( iter );
				return true;
			}
		}

		return false;
	}
	else
	{
		return remove( find_name_it->second );
	}
}

/**
*
*/
bool BaseCache::remove( const std::shared_ptr<v3d::BaseAsset>& asset )
{
	return false;
}

/**
*
*/
std::size_t BaseCache::purge()
{
	std::vector<std::size_t> purged;
	for (auto iter = assets.begin(); iter != assets.end();)
	{
		if ((iter->second) == nullptr || (iter->second).use_count() == 1)
		{
			purged.push_back( iter->first );
			iter = assets.erase( iter );
			continue;
		}

		iter++;
	}

	for (std::size_t purgedID : purged)
	{
		for (auto iter = nameMap.begin(); iter != nameMap.end();)
		{
			if (iter->second == purgedID)
			{
				iter = nameMap.erase( iter );
				break;
			}
			else iter++;
		}
	}

	return purged.size();
}

/**
*
*/
void BaseCache::log() const
{
	for (auto& asset : assets)
		(asset.second)->log();

	auto& logger = v3d::Logger::getInstance();
	logger.trace( "[BaseCache] Name:ID" );
	for (auto const& [key, val] : nameMap)
	{
		logger.trace( "{} : {}", key, val );
	}
}

/**
*
*/
void BaseCache::clear()
{
	assets.clear();
}

/**
*
*/
void BaseCache::clearKeys()
{
	nameMap.clear();
}

void BaseCache::clearAll()
{
	clear();
	clearKeys();
}

V3D_NS_END