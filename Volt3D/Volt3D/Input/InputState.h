/**
*	@file InputState.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_INPUT_STATE_H
#define V3D_INPUT_STATE_H

#include <string>

namespace v3d
{
	/**
	*	@enum InputStae
	*	State of input for keyboard and mouse.
	*/
	enum class InputState : bool
	{
		ePressed = 0,
		eReleased,
	};


	inline std::string inputStateToString(v3d::InputState value)
	{
		switch (value)
		{
		case v3d::InputState::ePressed: return "Pressed";
		case v3d::InputState::eReleased: return "Released";
		default: return "Invalid pivot";
		}
	}
}

#endif