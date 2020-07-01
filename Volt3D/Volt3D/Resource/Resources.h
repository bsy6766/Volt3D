/**
*	@file Resources.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_RESOURCES_H
#define V3D_RESOURCES_H

#include "Utils/Macros.h"

#include "ResourceManager.h"

V3D_NS_BEGIN

class ResourceManager;

/**
*	@class Resources
*	@brief A static wrapper to ResourceManager.
*/
class VOLT3D_DLL Resources
{
private:
	static ResourceManager rm;

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Resources);
	DEFAULT_MOVE_CONSTRUCTORS(Resources);

	Resources() = delete;
	~Resources() = delete;

	/**
	*	Get resource by asset ID.
	*	@param assetID An asset ID to get.
	*	@return 
	*/
	template<class T> std::shared_ptr<T> get(const unsigned int assetID) { return rm.get<T>(assetID); }
};

V3D_NS_END

#endif