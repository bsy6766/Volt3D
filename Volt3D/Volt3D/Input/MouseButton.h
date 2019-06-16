/**
*	@file MouseButton.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_MOUSE_BUTTON_H
#define V3D_MOUSE_BUTTON_H

#include <string>

namespace v3d
{
	/**
	*	@enum MouseButton
	*	A list of mouse buttons that remaps GLFW's mouse button define
	*/
	enum class MouseButton : unsigned int
	{
		e1 = 0,
		e2,
		e3,
		e4,
		e5,
		e6,
		e7,
		e8,
		eEnumLast,
		eLeft = e1,
		eRight = e2,
		eMiddle = e3,
	};

	inline std::string mouseButtonToString(v3d::MouseButton value)
	{
		switch (value)
		{
		case v3d::MouseButton::e1: return "1";
		case v3d::MouseButton::e2: return "2";
		case v3d::MouseButton::e3: return "3";
		case v3d::MouseButton::e4: return "4";
		case v3d::MouseButton::e5: return "5";
		case v3d::MouseButton::e6: return "6";
		case v3d::MouseButton::e7: return "7";
		case v3d::MouseButton::e8: return "8";
		case v3d::MouseButton::eEnumLast: return "EnumLast";
		default: return "Invalid mouse button";
		}
	}
}

#endif