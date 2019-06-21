/**
*	@file GamePadButton.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_GAMEPAD_BUTTON_H
#define V3D_GAMEPAD_BUTTON_H

#include <string>

namespace v3d
{
	namespace Gamepad
	{
		/**
		*	@enum Gamepad::Button
		*	@breif A list of gamepad button for xbox controllers mapped to SDL Gamecontroller button number.
		*/
		enum class Button
		{
			eA = 0,			// 0
			eB,				// 1
			eX,				// 2
			eY,				// 3
			eBack,			// 4
			eNone,			// 5
			eStart,			// 6
			eL3,				// 7 (Left stick)
			eR3,				// 8 (Right stick)
			eLS,				// 9 (Left shoulder)
			eRS,				// 10 (Right shoulder)
			eDpadUp,		// 11
			eDpadDown,		// 12
			eDpadLeft,		// 13
			eDpadRight,		// 14
			eEnumLast,
		};

		inline std::string gamepadButtonToString(v3d::Gamepad::Button value)
		{
			switch (value)
			{
			case v3d::Gamepad::Button:eA: return "A";
			case v3d::Gamepad::Button:eB: return "B";
			case v3d::Gamepad::Button:eX: return "X";
			case v3d::Gamepad::Button:eY: return "Y";
			case v3d::Gamepad::Button:eBack: return "Back";
			case v3d::Gamepad::Button:eNone: return "None";
			case v3d::Gamepad::Button:eStart: return "Start";
			case v3d::Gamepad::Button:eL3: return "L3";
			case v3d::Gamepad::Button:eR3: return "R3";
			case v3d::Gamepad::Button:eLS: return "LS";
			case v3d::Gamepad::Button:eRS: return "RS";
			case v3d::Gamepad::Button:eDpadUp: return "DpadUp";
			case v3d::Gamepad::Button:eDpadDown: return "DpadDown";
			case v3d::Gamepad::Button:eDpadLeft: return "DpadLeft";
			case v3d::Gamepad::Button:eDpadRight: return "DpadRight";
			case v3d::Gamepad::Button:eEnumLast: return "EnumLast";
			default: return "Invalid gamepad button";
			}
		}
	}
}

#endif
