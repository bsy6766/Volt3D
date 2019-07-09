/**
*	@file WindowMode.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_WINDOW_MODE_H
#define V3D_WINDOW_MODE_H

namespace v3d
{
	/**
	*	@enum WindowMode
	*	Defines 3 window modes.
	*	-1 means invlide window mode or unitialized state.
	*/
	enum class WindowMode : unsigned int
	{
		// Windowed: Window with border line, caption and system menu.
		eWindowed = 0,
		// Fullscreen: Fullscreen window. Minimizes when loses focus.
		eFullscreen,
		// Windowed fullscreen: Fullscreen window without border line, caption and system menu (a.k.a Borderless fullscreen).
		// Doesn't minimize when loses focus. 
		eWindowedFullscreen,
	};

	inline std::string windowModeToString(v3d::WindowMode value)
	{
		switch (value)
		{
		case v3d::WindowMode::eWindowed: return "Windowed";
		case v3d::WindowMode::eFullscreen: return "Fullscreen";
		case v3d::WindowMode::eWindowedFullscreen: return "Windowed Fullscreen";
		default: return "Invalid window mode type";
		}
	}

	static inline v3d::WindowMode toWindowModeEnum(const unsigned int value)
	{
		switch (value)
		{
		case 0: return v3d::WindowMode::eWindowed;
		case 1: return v3d::WindowMode::eFullscreen;
		case 2: return v3d::WindowMode::eWindowedFullscreen;
		default: return v3d::WindowMode::eWindowed;
		}
	}
}

#endif