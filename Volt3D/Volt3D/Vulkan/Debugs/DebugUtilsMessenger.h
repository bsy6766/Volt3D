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

V3D_NS_BEGIN
VK_NS_BEGIN

class Instance;

/**
*	@class DebugUtilsMessenger
*	@brief A wrapper of vulkan's DebugUtilsMessenger.
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL DebugUtilsMessenger
{
	friend class Instance;

private:
	vk::UniqueDebugUtilsMessengerEXT debugUtilsMessenger;

	static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugUtilsMessengerFunc
	(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);

	bool init( const vk::Instance& instance );

public:
	DebugUtilsMessenger();
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( DebugUtilsMessenger );
	DEFAULT_MOVE_CONSTRUCTORS( DebugUtilsMessenger );
	~DebugUtilsMessenger() {}
};

V3D_NS_END
VK_NS_END

#endif
