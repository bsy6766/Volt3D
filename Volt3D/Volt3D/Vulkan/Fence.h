/**
*	@file Fence.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_FENCE_H
#define V3D_VK_FENCE_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class Device;

		/**
		*	@class Fence
		*	@brief A wrapper for Vulkan's fence.
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Fence
		{
			friend class Context;

		private:
			Fence();

			vk::UniqueFence fence;

			bool init(const v3d::vulkan::Device& device);

		public:
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Fence);
			DEFAULT_MOVE_CONSTRUCTORS(Fence);
			~Fence() {}

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueFence, vk::Fence, fence);
		};
	}
}


#endif