/**
*	@file Surface.h
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
	namespace glfw { class Window; }
	namespace vulkan
	{
		class Instance;

		/**
		*	@class Surface
		*	@brief A wrapper of vulkan's surface
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Surface
		{
			friend class Context;

		private:
			vk::UniqueSurfaceKHR surface;

			bool init(const v3d::glfw::Window& window, const v3d::vulkan::Instance& instance);

		public:
			Surface();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Surface);
			DEFAULT_MOVE_CONSTRUCTORS(Surface);
			~Surface() {}

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueSurfaceKHR, vk::SurfaceKHR, surface);
		};
	}
}

#endif