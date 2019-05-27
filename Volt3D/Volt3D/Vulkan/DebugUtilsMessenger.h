/**
*	@file DebugUtilsMessenger.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_DEBUG_UTILS_MESSENGER_H
#define V3D_VK_DEBUG_UTILS_MESSENGER_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class Instance;

		/**
		*	@class DebugUtilsMessenger
		*	@brief A wrapper of vulkan's DebugUtilsMessenger.
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class DebugUtilsMessenger
		{
			friend class Context;

		private:
			vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger;

			static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugUtilsMessengerFunc
			(
				VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageType,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData
			);

			bool init(const v3d::vulkan::Instance& instance);

		public:
			DebugUtilsMessenger ();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(DebugUtilsMessenger);
			DEFAULT_MOVE_CONSTRUCTORS(DebugUtilsMessenger);
			~DebugUtilsMessenger () {}
		};
	}
}

#endif
