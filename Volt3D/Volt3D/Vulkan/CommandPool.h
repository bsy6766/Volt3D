/**
*	@file CommandPool.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_COMMAND_POOL_H
#define V3D_VK_COMMAND_POOL_H

#include <vulkan/vulkan.hpp>

#include <vector>

#include "Utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class PhysicalDevice;
		class Device;

		/**
		*	@class CommandPool
		*	@brief A wrapper for Vulkan's command pool.
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL CommandPool
		{
			friend class Context;
			friend class Device;

		private:
			vk::UniqueCommandPool commandPool;

			bool init(const v3d::vulkan::PhysicalDevice& physicalDevice, const v3d::vulkan::Device& device);

		public:
			CommandPool();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(CommandPool);
			DEFAULT_MOVE_CONSTRUCTORS(CommandPool);
			~CommandPool() {}

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueCommandPool, vk::CommandPool, commandPool);
		};
	}
}

#endif