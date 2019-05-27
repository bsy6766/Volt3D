/**
*	@file WindowMode.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_GLFW_WINDOW_MODE_H
#define V3D_GLFW_WINDOW_MODE_H

namespace v3d
{
	namespace glfw
	{
		/**
		*	@enum WindowMode
		*	Defines 3 window modes.
		*	-1 means invlide window mode or unitialized state.
		*/
		enum class WindowMode : unsigned int
		{
			// Windowed: Window with border line, caption and system menu.
			WINDOWED = 0,
			// Fullscreen: Fullscreen window. Minimizes when loses focus.
			FULLSCREEN,
			// Windowed fullscreen: Fullscreen window without border line, caption and system menu (a.k.a Borderless fullscreen).
			// Doesn't minimize when loses focus. 
			WINDOWED_FULLSCREEN,
			WINDOW_MODE_MAX_ENUM,
		};
			 
		static inline v3d::glfw::WindowMode toWindowModeEnum(const unsigned int windowModeInt)
		{
			if (windowModeInt == 0) return v3d::glfw::WindowMode::WINDOWED;
			else if (windowModeInt == 1) return v3d::glfw::WindowMode::FULLSCREEN;
			else if (windowModeInt == 2) return v3d::glfw::WindowMode::WINDOWED_FULLSCREEN;
			else
			{
				// @todo: throw error
				return v3d::glfw::WindowMode::WINDOWED;
			}
		}
	}
}

#endif