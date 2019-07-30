#ifndef V3D_INPUT_ENUMS_H
#define V3D_INPUT_ENUMS_H

#include <string>

namespace v3d
{

	// Gamepad
	namespace Gamepad
	{
		/**
		*	@enum Gamepad::Button
		*	A list of gamepad button for xbox controllers mapped to SDL Gamecontroller button number.
		*/
		enum class Button
		{
			A = 0,			// 0
			B,				// 1
			X,				// 2
			Y,				// 3
			BACK,			// 4
			NONE,			// 5
			START,			// 6
			L3,				// 7 (Left stick)
			R3,				// 8 (Right stick)
			LS,				// 9 (Left shoulder)
			RS,				// 10 (Right shoulder)
			DPAD_UP,		// 11
			DPAD_DOWN,		// 12
			DPAD_LEFT,		// 13
			DPAD_RIGHT,		// 14
			BUTTON_ENUM_LAST,
		};

		/**
		*	@enum Gamepad::Axis
		*	A list of gamepad axis for xbox controllers mapped to SDL Gamecontroller axis number.
		*/
		enum class Axis
		{
			L_AXIS_X = 0,		// Left stick X axis
			L_AXIS_Y,			// Left stick Y axis
			R_AXIS_X,			// Right stick X axis
			R_AXIS_Y,			// Right stick Y axis
			LT,					// Left trigger
			RT,					// Right trigger
			AXIS_ENUM_LAST,
		};
	}
}

#endif