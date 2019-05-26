﻿#ifndef WINDOW_H
#define WINDOW_H

#include "utils/Macros.h"

namespace v3d
{
	namespace glfw
	{
		class Window
		{
			friend class Engine;
		private:
			Window();

			GLFWwindow* window;

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

			bool createWindowSurface(vk::UniqueInstance& instance, VkSurfaceKHR& surface) const;

			std::size_t getGLFWVKExtensions(std::vector<const char*>& extensions) const;
		};
	}
}

#endif