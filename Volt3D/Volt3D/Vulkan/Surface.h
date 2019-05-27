/**
*	@file Instance.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_SURFACE_H
#define V3D_VK_SURFACE_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class Surface
		{
		private:
			vk::UniqueSurfaceKHR surface;

		public:
			Surface(vk::UniqueSurfaceKHR&& surface);
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Surface);
			DEFAULT_MOVE_CONSTRUCTORS(Surface);
			~Surface() {}

			CLASS_TO_VULKAN_HANDLE(vk::UniqueSurfaceKHR, surface);
		};
	}
}

#endif