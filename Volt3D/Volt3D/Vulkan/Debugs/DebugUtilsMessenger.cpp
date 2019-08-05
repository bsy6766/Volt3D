/**
*	@file DebugUtilsMessenger.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "DebugUtilsMessenger.h"

#include "Vulkan/Instance.h"

PFN_vkCreateDebugUtilsMessengerEXT	pfnVkCreateDebugUtilsMessengerEXT = VK_NULL_HANDLE;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT = VK_NULL_HANDLE;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback )
{
	return pfnVkCreateDebugUtilsMessengerEXT( instance, pCreateInfo, pAllocator, pCallback );
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT( VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator )
{
	pfnVkDestroyDebugUtilsMessengerEXT( instance, callback, pAllocator );
}

V3D_NS_BEGIN
VK_NS_BEGIN

DebugUtilsMessenger::DebugUtilsMessenger()
	: debugUtilsMessenger( nullptr )
{}

bool DebugUtilsMessenger::init( const vk::Instance& instance )
{
	auto& logger = v3d::Logger::getInstance();

	pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr( "vkCreateDebugUtilsMessengerEXT" ));
	pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr( "vkDestroyDebugUtilsMessengerEXT" ));

	if (!pfnVkCreateDebugUtilsMessengerEXT)
	{
		logger.error( "vkCreateDebugUtilsMessengerEXT is not supported" );
		return false;
	}
	if (!pfnVkDestroyDebugUtilsMessengerEXT)
	{
		logger.error( "vkDestroyDebugUtilsMessengerEXT is not supported" );
		return false;
	}

	vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity( vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError );
	vk::DebugUtilsMessageTypeFlagsEXT typeFlags( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance );
	debugUtilsMessenger = std::move( instance.createDebugUtilsMessengerEXTUnique( vk::DebugUtilsMessengerCreateInfoEXT( {}, messageSeverity, typeFlags, debugUtilsMessengerFunc, this ) ) );

	return true;
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugUtilsMessenger::debugUtilsMessengerFunc( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData )
{
	Logger::getInstance().debug( std::string( "Debug Util Messeage: " ) + std::string( pCallbackData->pMessage ) );
	return VK_FALSE;
}

/*
vk::Bool32 v3d::vulkan::DebugUtilsMessenger::debugReportCallback( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char*, const char* pMessage, void* )
{
	switch( flags )
	{
	case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
		std::cerr << "INFORMATION: ";
		break;
	case VK_DEBUG_REPORT_WARNING_BIT_EXT:
		std::cerr << "WARNING: ";
		break;
	case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
		std::cerr << "PERFORMANCE WARNING: ";
		break;
	case VK_DEBUG_REPORT_ERROR_BIT_EXT:
		std::cerr << "ERROR: ";
		break;
	case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
		std::cerr << "DEBUG: ";
		break;
	default:
		std::cerr << "unknown flag (" << flags << "): ";
		break;
	}
	std::cerr << pMessage << std::endl;
	return VK_TRUE;
}
	*/

V3D_NS_END
VK_NS_END