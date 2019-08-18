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

V3D_NS_BEGIN
VK_NS_BEGIN

class DebugReportCallback;
class DebugUtilsMessenger;

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
	Instance();

	vk::Instance instance;

	v3d::vulkan::DebugReportCallback* debugReportCallback;
	v3d::vulkan::DebugUtilsMessenger* debugUtilsMessenger;

	bool init( std::vector<const char*>& requiredExtensions, const bool validationLayerEnabled );
	bool initDebugReport();
	bool initDebugUtilsMessenger();

	PFN_vkVoidFunction getProcAddr( const char* pName ) const;
	vk::UniqueDebugReportCallbackEXT createDebugReportCallbackEXTUnique( const vk::DebugReportCallbackCreateInfoEXT& createInfo ) const;
	vk::UniqueDebugUtilsMessengerEXT createDebugUtilsMessengerEXTUnique( const vk::DebugUtilsMessengerCreateInfoEXT& createInfo ) const;

public:
	~Instance();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Instance );
	DEFAULT_MOVE_CONSTRUCTORS( Instance );
	
	/** Get vulkan instance */
	const vk::Instance& getVKInstance() const;
	std::vector<vk::PhysicalDevice> enumeratePhysicalDevices() const;
};

VK_NS_END
V3D_NS_END

#endif