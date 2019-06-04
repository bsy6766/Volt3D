/**
*	@file Semaphore.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_SEMAPHORE_H
#define V3D_VK_SEMAPHORE_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class Device;

		/**
		*	@class Semaphore
		*	@brief Wrapper for Vulkan's semaphore.
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Semaphore
		{
			friend class Context;

		private:
			vk::UniqueSemaphore semaphore;

			bool init(const v3d::vulkan::Device& device);
		public:
			Semaphore();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Semaphore);
			DEFAULT_MOVE_CONSTRUCTORS(Semaphore);
			~Semaphore() {};

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueSemaphore, vk::Semaphore, semaphore);
		};
	}
}

#endif