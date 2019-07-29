/**
*	@file Devices.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_PHYSICAL_DEVICE_H
#define V3D_VK_PHYSICAL_DEVICE_H

#include <vulkan/vulkan.hpp>

#include <optional>
#include <vector>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		/**
		*	@class Device
		*	@brief A wrapper of vulkan's physical device and logical device
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Devices
		{
			friend class Context;

		private:
			vk::PhysicalDevice physicalDevice;
			vk::Device logicalDevice;
			VkPhysicalDeviceProperties properties;
			VkPhysicalDeviceFeatures features;
			//VkPhysicalDeviceFeatures enabledFeatures;
			VkPhysicalDeviceMemoryProperties memoryProperties;
			std::vector<vk::QueueFamilyProperties> queueFamilyProperties;

			struct QueueFamilyIndex
			{
				uint32_t graphics;
				uint32_t compute;
				uint32_t transfer;
				uint32_t present;
			}QFI;
			
			bool isSuitablePhysicalDevice( const vk::PhysicalDevice& physicalDevice );
			bool initPhysicalDevice( const std::vector<vk::PhysicalDevice>& physicalDevices );
			bool initLogicalDevice( const vk::SurfaceKHR& surface );
			bool initQueueFamilyIndices( const vk::SurfaceKHR& surface );

			bool isSuitable( const vk::PhysicalDevice& physicalDevice );

		public:
			Devices();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Devices );
			DEFAULT_MOVE_CONSTRUCTORS( Devices );
			~Devices() {}

			vk::Device createDeviceUnique( vk::DeviceCreateInfo& createInfo ) const;
			std::vector<vk::QueueFamilyProperties> getQueueFamilyProperties() const;
			vk::SurfaceCapabilitiesKHR getSurfaceCapabilitiesKHR( const vk::SurfaceKHR& surface ) const;
			vk::Bool32 getSurfaceSupportKHR( const uint32_t index, const vk::SurfaceKHR& surface ) const;
			std::vector<vk::SurfaceFormatKHR> getSurfaceFormatsKHR( const vk::SurfaceKHR& surface ) const;
			std::vector<vk::PresentModeKHR> getSurfacePresentModesKHR( const vk::SurfaceKHR& surface ) const;
			vk::PhysicalDeviceProperties getProperties() const;
			vk::PhysicalDeviceFeatures getFeatures() const;
			vk::PhysicalDeviceMemoryProperties getMemoryProperties() const;
			std::vector<vk::ExtensionProperties> EnumerateDeviceExtensionProperties() const;
			std::vector<vk::LayerProperties> enumerateDeviceLayerProperties() const;
			uint32_t getGraphicsQueueFamilyIndex() const;
			uint32_t getComputeQueueFamilyIndex() const;
			uint32_t getTransferQueueFamilyIndex() const;
			uint32_t getPresentQueueFamilyIndex() const;
			uint32_t getMemoryTypeIndex( const uint32_t memoryTypeBits, const vk::MemoryPropertyFlags memoryPropertyFlags ) const;
		};
	}
}

#endif