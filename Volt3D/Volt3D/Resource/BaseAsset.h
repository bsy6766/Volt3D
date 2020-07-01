/**
*	@file BaseAsset.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_BASE_ASSET_H
#define V3D_BASE_ASSET_H

#include "Utils/Macros.h"

#include <string>

V3D_NS_BEGIN

/**
*	@class BaseAsset	
*	@brief A base class for all assets in the engine
*
*	Contains id and name.
*
*	If ID is 0, it means this asset is not managed by the engine,
*	because all assets which are managed (cached) by the engine will
*	receive and get assigned with unique ID (std::size_t).
*
*	Name should not be empty. Name must be give always as unique key for an object.
*	
*	@group Volt3D
*
*	@since 1.0
*/
class VOLT3D_DLL BaseAsset
{
private:
	BaseAsset() = delete;

protected:
	BaseAsset( const std::string& name );
	
	std::size_t id;
	std::string name;

	bool assignID( const std::size_t id );
	bool isManaged() const;

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( BaseAsset );
	DEFAULT_MOVE_CONSTRUCTORS( BaseAsset );

	virtual ~BaseAsset();

	/** Get ID of this asset. */
	virtual std::size_t getID() const;

	/** Get name of this asset. */
	virtual std::string getName() const;

	/** Log this asset. */
	virtual void log() const;
};

V3D_NS_END

#endif