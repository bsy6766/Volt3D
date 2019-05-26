#include <PreCompiled.h>

#include "DebugCallback.h"

#include "Logger.h"



PFN_vkCreateDebugReportCallbackEXT  pfnVkCreateDebugReportCallbackEXT = VK_NULL_HANDLE;
PFN_vkDestroyDebugReportCallbackEXT pfnVkDestroyDebugReportCallbackEXT = VK_NULL_HANDLE;
PFN_vkCreateDebugUtilsMessengerEXT	pfnVkCreateDebugUtilsMessengerEXT = VK_NULL_HANDLE;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT = VK_NULL_HANDLE;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugReportCallbackEXT( VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback )
{
	return pfnVkCreateDebugReportCallbackEXT( instance, pCreateInfo, pAllocator, pCallback );
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugReportCallbackEXT( VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator )
{
	pfnVkDestroyDebugReportCallbackEXT( instance, callback, pAllocator );
}

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback )
{
	return pfnVkCreateDebugUtilsMessengerEXT( instance, pCreateInfo, pAllocator, pCallback );
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT( VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator )
{
	pfnVkDestroyDebugUtilsMessengerEXT( instance, callback, pAllocator );
}




DebugCallback::DebugCallback()
	: drcb(nullptr)
	, dub(nullptr)
{}

bool DebugCallback::init( vk::UniqueInstance & instance )
{
	auto& logger = Logger::getInstance();

	pfnVkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(instance->getProcAddr( "vkCreateDebugReportCallbackEXT" ));
	pfnVkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(instance->getProcAddr( "vkDestroyDebugReportCallbackEXT" ));
	
	if( !pfnVkCreateDebugReportCallbackEXT )
	{
		logger.error( "vkCreateDebugReportCallbackEXT is not supported" );
		return false;
	}
	if( !pfnVkDestroyDebugReportCallbackEXT )
	{
		logger.error( "vkDestroyDebugReportCallbackEXT is not supported" );
		return false;
	}

	vk::DebugReportFlagsEXT flags( vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::ePerformanceWarning | vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eDebug );
	drcb = instance->createDebugReportCallbackEXTUnique( vk::DebugReportCallbackCreateInfoEXT( flags, debugReportCbFunc, this ) );

	pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance->getProcAddr( "vkCreateDebugUtilsMessengerEXT" ));
	pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance->getProcAddr( "vkDestroyDebugUtilsMessengerEXT" ));
	
	if( !pfnVkCreateDebugUtilsMessengerEXT )
	{
		logger.error( "vkCreateDebugUtilsMessengerEXT is not supported" );
		return false;
	}
	if( !pfnVkDestroyDebugUtilsMessengerEXT )
	{
		logger.error( "vkDestroyDebugUtilsMessengerEXT is not supported" );
		return false;
	}

	vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity( vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError );
	vk::DebugUtilsMessageTypeFlagsEXT typeFlags( vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance );
	dub = instance->createDebugUtilsMessengerEXTUnique( vk::DebugUtilsMessengerCreateInfoEXT( {}, messageSeverity, typeFlags, debugUtilsMessengerFunc, this ) );

	return true;
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugCallback::debugReportCbFunc( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT object_type, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData )
{
	Logger::getInstance().debug( std::string( "Debug report: " ) + std::string( pMessage ));
	return VK_FALSE;
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugCallback::debugUtilsMessengerFunc( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData )
{
	Logger::getInstance().debug( std::string( "Debug Util Messeage: " ) + std::string( pCallbackData->pMessage ) );
	return VK_FALSE;
}

	/*
vk::Bool32 DebugCallback::debugReportCallback( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char*, const char* pMessage, void* )
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