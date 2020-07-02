/**
*	@file ResourceManager.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "ResourceManager.h"

#include "Shader/ShaderCache.h"
#include "Shader/Shader.h"
#include "Texture/TextureCache.h"
#include "Texture/Texture2D.h"

V3D_NS_BEGIN

ResourceManager::ResourceManager()
	: cacheIndices()
	, caches()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info("[ResourceManager] Created");
#endif
}

ResourceManager::~ResourceManager()
{
	releaseAll();
}

inline std::shared_ptr<v3d::BaseAsset> ResourceManager::getAsset(const std::type_index ti, const std::size_t id)
{ 
	auto findCacheIdx = cacheIndices.find(ti);
	if (findCacheIdx == cacheIndices.end()) return nullptr;

	auto findCache = caches.find(findCacheIdx->second);
	if (findCache == caches.end()) return nullptr;

	return (findCache->second)->get(id);
}

bool ResourceManager::init()
{
	if (!initCacheIndices()) return false;
	if (!initCaches()) return false;
	if (!initLoaders()) return false;
	if (!initDefaultResources()) return false;

#ifdef BUILD_DEBUG
	auto& logger = v3d::Logger::getInstance();
	logger.info("[ResourceManager] Initialized");
	logger.info("[ResourceManager] Asset Caches Indices...");
	for (auto& e : cacheIndices)
	{
		logger.info("[{}({})] -> {}({})", e.first.name(), e.first.hash_code(), e.second.name(), e.second.hash_code());
	}
	logger.info("[ResourceManager] Total {} caches loaded", caches.size());
#endif

	return true;
}

bool ResourceManager::initCacheIndices()
{
	// Register all default resources with cache

	// textures
	const auto textureCacheTypeIndex = std::type_index(typeid(v3d::TextureCache));
	cacheIndices.emplace(std::pair<std::type_index, std::type_index>(std::type_index(typeid(v3d::Texture)), textureCacheTypeIndex));
	cacheIndices.emplace(std::pair<std::type_index, std::type_index>(std::type_index(typeid(v3d::Texture2D)), textureCacheTypeIndex));
	//cacheIndices.emplace(std::pair<std::type_index, std::type_index>(std::type_index(typeid(v3d::Texture3D)), textureCacheTypeIndex));
	//cacheIndices.emplace(std::pair<std::type_index, std::type_index>(std::type_index(typeid(v3d::Texture1D)), textureCacheTypeIndex));

	// Shaders
	const auto shaderCacheTypeIndex = std::type_index(typeid(v3d::ShaderCache));
	cacheIndices.emplace(std::pair<std::type_index, std::type_index>(std::type_index(typeid(v3d::Shader)), shaderCacheTypeIndex));

	// Audios

	// etc...

	// Validation
#ifdef BUILD_DEBUG
	CHECKERROR_CLS(cacheIndices.find(std::type_index(typeid(v3d::Texture))) != cacheIndices.end(), "Failed to find TextureCache's cache index.", v3d::ResourceManager);
	CHECKERROR_CLS(cacheIndices.find(std::type_index(typeid(v3d::Texture2D))) != cacheIndices.end(), "Failed to find TextureCache's cache index.", v3d::ResourceManager);
	CHECKERROR_CLS(cacheIndices.find(std::type_index(typeid(v3d::Shader))) != cacheIndices.end(), "Failed to find ShaderCache's cache index.", v3d::ResourceManager);
#endif

	return true;
}

bool ResourceManager::initCaches()
{
	// Init caches
	//v3d::TextureCache* textureCache = new (std::nothrow) v3d::TextureCache();
	//if (!textureCache) return false;
	//std::unique_ptr<v3d::BaseCache> tc_uptr = std::unique_ptr<v3d::TextureCache>(textureCache);
	//caches[textureCacheTypeIndex] = std::move(tc_uptr);
	//caches.emplace(textureCacheTypeIndex, std::unique_ptr<v3d::TextureCache>(textureCache));

	//v3d::ShaderCache* shaderCache = new (std::nothrow) v3d::ShaderCache();
	//if (!shaderCache) return false;
	//caches.emplace(shaderCacheTypeIndex, std::unique_ptr<v3d::ShaderCache>(shaderCache));

	return true;
}

bool ResourceManager::initLoaders()
{
	return false;
}

bool ResourceManager::initDefaultResources()
{
	return false;
}

void ResourceManager::releaseAll()
{
	for (auto& e : caches)
		(e.second)->clearAll();
	// Manually release all caches
	for (auto& e : caches)
	{
		auto& cache = e.second;
		auto cachePtr = cache.release();
		SAFE_DELETE(cachePtr);
		cache.reset();
	}
	caches.clear();
}

V3D_NS_END
