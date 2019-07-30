/**
*	@file CursorMode.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_CURSOR_MODE_H
#define V3D_CURSOR_MODE_H

#include <string>

namespace v3d
{
	/**
	*	@enum CursorMode
	*	@brief Defines 3 different cursor mode
	*
	*	@since 1.0
	*/
	enum class CursorMode : int
	{
		// Normal mode (default). Cursor is shown.
		eNormal = 0,
		// Hidden mode. Cursor is hidden when it's above application's client area.
		eHidden,
		// Disabled mode. Cursor is hidden and locked in window. 
		eDisabled,
	};

	inline std::string cursorModeToString(v3d::CursorMode value)
	{
		switch (value)
		{
		case v3d::CursorMode::eNormal: return "Normal";
		case v3d::CursorMode::eHidden: return "Hidden";
		case v3d::CursorMode::eDisabled: return "Disabled";
		default: return "Invalid cursor mode";
		}
	}
}

#endif