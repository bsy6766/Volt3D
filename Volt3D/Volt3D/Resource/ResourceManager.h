/**
*	@file ResourceManager.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_RESOURCE_MANAGER_H
#define V3D_RESOURCE_MANAGER_H

#include "Utils/Macros.h"

#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include "BaseAsset.h"
#include "BaseCache.h"

V3D_NS_BEGIN

class BaseAsset;
class BaseCache;

/**
*	@class ResourceManager
*/
class VOLT3D_DLL ResourceManager
{
	friend class Engine;

private:
	std::unordered_map<std::type_index, std::type_index> cacheIndices;
	std::unordered_map<std::type_index, std::unique_ptr<v3d::BaseCache>> caches;

	inline std::shared_ptr<v3d::BaseAsset> getAsset(const std::type_index ti, const std::size_t id);

private:
	/** Check if passed template is base of v3d::BaseAsset */
	template<class T>
	inline bool isAsset()
	{
		if constexpr (std::is_base_of_v<v3d::BaseAsset, T>) return true;
		else												return false;
	}

	/** Initialize ResourceManager */
	bool init();
	bool initCacheIndices();
	bool initCaches();
	bool initLoaders();
	bool initDefaultResources();

	template<class T>
	std::shared_ptr<T> get(const unsigned int id)
	{
		if (id == 0) return nullptr;
		if (!isAsset<T>()) return nullptr;

		std::shared_ptr<v3d::BaseAsset> asset = getAsset(std::type_index(typeid(T)), id);

		if (asset)	return std::dynamic_pointer_cast<T>(asset);
		else		return nullptr;
	}

	template<class T>
	std::shared_ptr<T> get(const std::string& name)
	{
		return nullptr;
	}

	template<class T>
	std::shared_ptr<T> load(const std::string& name, const std::string& filePath)
	{
		if (filePath.empty()) return nullptr;
		if (name.empty()) return nullptr;
	}
public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(ResourceManager);
	DEFAULT_MOVE_CONSTRUCTORS(ResourceManager);

	ResourceManager();
	~ResourceManager();

	void releaseAll();
};

V3D_NS_END

#endif