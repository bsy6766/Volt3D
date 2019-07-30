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
			friend class Context;

		private:
			vk::UniqueInstance instance;

			bool init( std::vector<const char*>& requiredExtensions, const bool validationLayerEnabled );

		public:
			Instance();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Instance );
			DEFAULT_MOVE_CONSTRUCTORS( Instance );
			~Instance() {}

			UNIQUE_TO_CPP_VK_HANDLE( vk::UniqueInstance, vk::Instance, instance );

			inline PFN_vkVoidFunction getProcAddr( const char* pName ) const;
			inline vk::UniqueDebugReportCallbackEXT createDebugReportCallbackEXTUnique( const vk::DebugReportCallbackCreateInfoEXT& createInfo ) const;
			inline vk::UniqueDebugUtilsMessengerEXT createDebugUtilsMessengerEXTUnique( const vk::DebugUtilsMessengerCreateInfoEXT& createInfo ) const;
			inline std::vector<vk::PhysicalDevice> enumeratePhysicalDevices() const;
		};
	}
}

#endif