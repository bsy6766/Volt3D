/**
*	@file Utils.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_UTILS_H
#define V3D_VK_UTILS_H

#include <vulkan/vulkan.hpp>

#include <vector>

#include "Utils/Logger.h"

namespace v3d
{
	namespace vulkan
	{
		namespace utils
		{
			static std::vector<vk::ExtensionProperties> getExtensions()
			{
				return vk::enumerateInstanceExtensionProperties();
			}

			static std::vector<vk::LayerProperties> getLayers()
			{
				return vk::enumerateInstanceLayerProperties();
			}

			static bool checkLayerProperty(std::vector<vk::LayerProperties>& layers, const char* requiredLayer)
			{
				for (const auto& layer : layers)
				{
					if (strcmp(requiredLayer, layer.layerName) == 0) { return true; }
				}

#if _DEBUG
				v3d::Logger::getInstance().critical("Missing Context Layer: " + std::string(requiredLayer));
#endif
				return false;
			}

			static bool checkLayerProperties(std::vector<vk::LayerProperties>& layers, const std::vector<const char*>& requiredLayers)
			{
				for (const char* requiredLayer : requiredLayers)
				{
					bool found = false;

					for (const auto& layer : layers)
					{
						if (strcmp(requiredLayer, layer.layerName) == 0) { found = true; break; }
					}

					if (!found)
					{
#if _DEBUG
						v3d::Logger::getInstance().critical("Missing Context Layer: " + std::string(requiredLayer));
#endif
						return false;
					}
				}

				return true;
			}

			static bool checkExtensionProperty(std::vector<vk::ExtensionProperties>& extensions, const char* requiredExtension)
			{
				for (const auto& extension : extensions)
				{
					if (strcmp(requiredExtension, extension.extensionName) == 0) { return true; }
				}

#if _DEBUG
				v3d::Logger::getInstance().critical("Missing Context Extension: " + std::string(requiredExtension));
#endif
				return false;
			}

			static bool checkExtensionProperties(std::vector<vk::ExtensionProperties>& extensions, const std::vector<const char*> requiredExtensions)
			{
				for (const char* requiredExtension : requiredExtensions)
				{
					bool found = false;

					for (const auto& extension : extensions)
					{
						if (strcmp(requiredExtension, extension.extensionName) == 0) { found = true; break; }
					}

					if (!found)
					{
#if _DEBUG
						v3d::Logger::getInstance().critical("Missing Context Extension: " + std::string(requiredExtension));
#endif
						return false;
					}
				}

				return true;
			}

			static uint32_t getVersion()
			{
				uint32_t version = 0;
				if (vk::enumerateInstanceVersion(&version) == vk::Result::eSuccess) return version;
				return 0;
			}

			static bool getVersion(uint32_t& major, uint32_t& minor, uint32_t& patch, uint32_t version)
			{
				if (version == 0) version = getVersion();
				if (version == 0) { major = 0; minor = 0; patch = 0; return false; }
				major = VK_VERSION_MAJOR(version);
				minor = VK_VERSION_MINOR(version);
				patch = VK_VERSION_PATCH(VK_HEADER_VERSION);
				return true;
			}

			static bool getVersion(uint32_t& major, uint32_t& minor, uint32_t& patch)
			{
				return getVersion(major, minor, patch, getVersion());
			}
		}
	}
}

#endif