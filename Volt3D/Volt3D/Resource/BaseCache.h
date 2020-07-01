/**
*	@file BaseCache.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_BASE_CACHE_H
#define V3D_BASE_CACHE_H

#include "Utils/Macros.h"

#include <unordered_map>
#include <memory>
#include <string>
#include <string_view>

#include "BaseAsset.h"

#include "Utils/Logger.h"

V3D_NS_BEGIN

class VOLT3D_DLL BaseCache
{
protected:
	BaseCache();
	
	std::unordered_map<std::string/*Name*/, std::size_t/*ID*/> nameMap;
	std::unordered_map<std::size_t/*ID*/, std::shared_ptr<v3d::BaseAsset>/*Asset*/> assets;

	virtual bool isSupported(std::type_info& typeInfo) = 0;

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( BaseCache );
	DEFAULT_MOVE_CONSTRUCTORS( BaseCache );

	virtual ~BaseCache();

	/**
	*	Check if this cache has asset with given id
	*/
	virtual bool has( const std::size_t id ) const;

	/**
	*
	*/
	virtual bool has( const std::string& name ) const;

	/**
	*
	*/
	virtual bool has( const std::shared_ptr<v3d::BaseAsset>& asset ) const;

	/**
	*
	*/
	virtual std::shared_ptr<v3d::BaseAsset> get( const std::size_t id ) const;

	/**
	*
	*/
	virtual std::shared_ptr<v3d::BaseAsset> get( const std::string& name ) const;

	/**
	*
	*/
	virtual std::vector<std::shared_ptr<v3d::BaseAsset>> getAll() const;

	/**
	*
	*/
	virtual bool remove( const std::size_t id );

	/**
	*
	*/
	virtual bool remove( const std::string& name );

	/**
	*
	*/
	virtual bool remove( const std::shared_ptr<v3d::BaseAsset>& asset );

	/**
	*
	*/
	virtual std::size_t purge();

	/**
	*
	*/
	virtual void log() const;

	/**
	*
	*/
	virtual void clear();

	/**
	*
	*/
	virtual void clearKeys();

	virtual void clearAll();
};

V3D_NS_END

#endif