/**
*	@file Window.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_GLFW_WINDOW_H
#define V3D_GLFW_WINDOW_H

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include <vector>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan { class Instance; }
	namespace glfw
	{
		/**
		*	@class Window
		*	@brief Simple wrapper for GLFW
		*
		*	@group GLFW
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Window
		{
			friend class Engine;
			friend class vulkan::Instance;

		private:
			Window();

			GLFWwindow* window;

			bool vsync;

			std::size_t getGLFWVKExtensions(std::vector<const char*>& extensions) const;

		public:
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Window);
			DEFAULT_MOVE_CONSTRUCTORS(Window);
			~Window();

			bool initGLFW();
			bool initWindow(const std::string_view windowTitle);
			void initGLFWHints();
			bool isRunning();
			void pollGLFWEvent();
			bool closeWindow();
			void releaseGLFW();

			bool createWindowSurface(const v3d::vulkan::Instance& instance, VkSurfaceKHR& surface) const;

			void setVsync(const bool enabled);
			bool isVsyncEnabled() const;

			bool isIconified() const;
			glm::uvec2 getFrameBufferSize() const;
		};
	}
}

#endif