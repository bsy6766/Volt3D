#ifndef DEBUG_CALLBACK_H
#define DEBUG_CALLBACK_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

class DebugCallback
{
	friend class Vulkan;
private:
	DebugCallback();

	vk::UniqueDebugReportCallbackEXT drcb;
	vk::UniqueDebugUtilsMessengerEXT dub;

	bool init( vk::UniqueInstance& instance );

	static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugReportCbFunc( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT object_type, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData );
	static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugUtilsMessengerFunc( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData );

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( DebugCallback );
	DEFAULT_MOVE_CONSTRUCTORS( DebugCallback );

	~DebugCallback() {}
};

#endif