/**
*	@file KeyModifier.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_KEY_MODIFIER_H
#define V3D_KEY_MODIFIER_H

#include <string>

namespace v3d
{
	/**
	*	@enum KeyModifier
	*	Bit flag for key modifiers
	*
	*	0: Idle
	*	1: Shift only
	*	2: Control only
	*	3: Shift + Control
	*	4: Alt only
	*	5: Shift + Alt
	*	6: Control + Alt
	*	7: Shift + Control + Alt
	*	8: Super only (OS key)
	*	9: Shift + Super
	*	10: Control + Super
	*	11: Shift + Control + Super
	*	12: Alt + Super
	*	13: Shift + Alt + Super
	*	14: Control + Alt + Super
	*	15: Shift + Control + Alt + Super
	*	16: CapsLock only
	*	32: NumLock only
	*/
	enum KeyModifierBits : unsigned int
	{
		eNone = 0,				// 0
		eShift = 1 << 0,			// 1
		eControl = 1 << 1,		// 2
		eAlt = 1 << 2,			// 4
		eSuper = 1 << 3,			// 8
		eCapsLock = 1 << 4,		// 16
		eNumLock = 1 << 5,		// 32
	};

	inline std::string keyModifierBitsToString(v3d::KeyModifierBits value)
	{
		switch (value)
		{
		case v3d::KeyModifierBits::eNone: return "None";
		case v3d::KeyModifierBits::eShift: return "Shift";
		case v3d::KeyModifierBits::eControl: return "Control";
		case v3d::KeyModifierBits::eAlt: return "Alt";
		case v3d::KeyModifierBits::eSuper: return "Super";
		case v3d::KeyModifierBits::eCapsLock: return "CapsLock";
		case v3d::KeyModifierBits::eNumLock: return "NumLock";
		default: return "Invalid key modifier";
		}
	}
}

#endif