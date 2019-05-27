/**
*	@file Instance.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_INSTANCE_H
#define V3D_VK_INSTANCE_H

#include <vulkan/vulkan.hpp>

#include <vector>

#include "Utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		/**
		*	@class Instance
		*	@brief Wrapper for VkInstance
		*
		*	@since 1.0	
		*/
		class VOLT3D_DLL Instance
		{
		private:
			vk::UniqueInstance instance;

		public:
			Instance(vk::UniqueInstance&& instance);
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Instance);
			DEFAULT_MOVE_CONSTRUCTORS(Instance);
			~Instance() {}

			CLASS_TO_VULKAN_HANDLE(vk::UniqueInstance, instance);

			inline PFN_vkVoidFunction getProcAddr(const char* pName) const;
			inline vk::UniqueDebugReportCallbackEXT createDebugReportCallbackEXTUnique(const vk::DebugReportCallbackCreateInfoEXT& createInfo) const;
			inline vk::UniqueDebugUtilsMessengerEXT createDebugUtilsMessengerEXTUnique(const vk::DebugUtilsMessengerCreateInfoEXT& createInfo) const;
			inline std::vector<vk::PhysicalDevice> enumeratePhysicalDevices() const;
		};
	}
}

#endif