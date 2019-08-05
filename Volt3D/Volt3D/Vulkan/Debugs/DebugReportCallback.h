﻿/**
*	@file DebugReportCallback.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_DEBUG_REPORT_CALLBACK_H
#define V3D_VK_DEBUG_REPORT_CALLBACK_H

#include <vulkan/vulkan.hpp>

#include "Utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

/**
*	@class DebugReportCallback
*	@brief A wrapper of vulkan's DebugReportCallback.
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL DebugReportCallback
{
	friend class Instance;

private:
	vk::UniqueDebugReportCallbackEXT debugReportCallback;

	static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugReportCallbackFunc
	(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT object_type,
		uint64_t object,
		size_t location,
		int32_t messageCode,
		const char* pLayerPrefix,
		const char* pMessage,
		void* pUserData
	);

	bool init( const vk::Instance& instance );

public:
	DebugReportCallback();
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( DebugReportCallback );
	DEFAULT_MOVE_CONSTRUCTORS( DebugReportCallback );
	~DebugReportCallback() {}
};

VK_NS_END
V3D_NS_END

#endif