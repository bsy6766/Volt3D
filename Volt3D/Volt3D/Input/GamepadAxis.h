/**
*	@file GamePadAxis.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_GAMEPAD_AXIS_H
#define V3D_GAMEPAD_AXIS_H

#include <string>

namespace v3d
{
	namespace Gamepad
	{
		/**
		*	@enum Gamepad::Axis
		*	@brief A list of gamepad axis for xbox controllers mapped to SDL Gamecontroller axis number.
		*
		*	@since 1.0
		*/
		enum class Axis
		{
			eLeftAxisX = 0,			// Left stick X axis
			eLeftAxisY,				// Left stick Y axis
			eRightAxisX,			// Right stick X axis
			eRightAxisY,			// Right stick Y axis
			eLT,					// Left trigger
			eRT,					// Right trigger
			eEnumLast,
		};

		inline std::string gamepadAxisToString(v3d::Gamepad::Axis value)
		{
			switch (value)
			{
			case v3d::Gamepad::Axis::eLeftAxisX: return "LeftAxisX";
			case v3d::Gamepad::Axis::eLeftAxisY: return "LeftAxisY";
			case v3d::Gamepad::Axis::eRightAxisX: return "RightAxisX";
			case v3d::Gamepad::Axis::eRightAxisY: return "RightAxisY";
			case v3d::Gamepad::Axis::eLT: return "LT";
			case v3d::Gamepad::Axis::eRT: return "RT";
			case v3d::Gamepad::Axis::eEnumLast: return "EnumLast";
			default: return "Invalid gamepad Axis";
			}
		}
	}
}

#endif
