/**
*	@file KeyCode.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_KEY_CODE_H
#define V3D_KEY_CODE_H

#include <string>

namespace v3d
{
	/**
	*	@enum KeyCode
	*	A list of key code that remaps GLFW's key define.
	*/
	enum class KeyCode : int
	{
		//eUnknown = -1,
		// Printable keys
		eSpace,
		eApostrophe,
		eComma,
		eMinus,
		ePeriod,
		eSlash,
		e0,
		e1,
		e2,
		e3,
		e4,
		e5,
		e6,
		e7,
		e8,
		e9,
		eSemicolon,
		eEqual,
		eA,
		eB,
		eC,
		eD,
		eE,
		eF,
		eG,
		eH,
		eI,
		eJ,
		eK,
		eL,
		eM,
		eN,
		eO,
		eP,
		eQ,
		eR,
		eS,
		eT,
		eU,
		eV,
		eW,
		eX,
		eY,
		eZ,
		eLeftBracket,
		eBackslash,
		eRightBracket,
		eGraveAccent,
		eWorld1,			/* non-US #1 */
		eWorld2,			/* non-US #2 */
		// Function keys
		eEscape,
		eEnter,
		eTab,
		eBackspace,
		eInsert,
		eDelete,
		eRight,
		eLeft,
		eDown,
		eUp,
		ePageUp,
		ePageDown,
		eHome,
		eEnd,
		eCapsLock,
		eScrollLock,
		eNumLock,
		ePrintScreen,
		ePause,
		eF1,
		eF2,
		eF3,
		eF4,
		eF5,
		eF6,
		eF7,
		eF8,
		eF9,
		eF10,
		eF11,
		eF12,
		eF13,
		eF14,
		eF15,
		eF16,
		eF17,
		eF18,
		eF19,
		eF20,
		eF21,
		eF22,
		eF23,
		eF24,
		eF25,
		eKP0,
		eKP1,
		eKP2,
		eKP3,
		eKP4,
		eKP5,
		eKP6,
		eKP7,
		eKP8,
		eKP9,
		eKPDecimal,
		eKPDivide,
		eKPMultiply,
		eKPSubtract,
		eKPAdd,
		eKPEnter,
		eKPEqual,
		eLeftShift,
		eLeftControl,
		eLeftAlt,
		eLeftSuper,		/* Left OS key */
		eRightShift,
		eRightControl,
		eRightAlt,
		eRightSuper,		/* Right OS key */
		eMenu,
		// Last
		eEnumLast,
	};

	inline std::string keyCodeToString(v3d::KeyCode value)
	{
		switch (value)
		{
		//case v3d::KeyCode::eUnknown: return "Unknown";
		case v3d::KeyCode::eSpace: return "Space";
		case v3d::KeyCode::eApostrophe: return "Apostrophe";
		case v3d::KeyCode::eComma: return "Comma";
		case v3d::KeyCode::eMinus: return "Minus";
		case v3d::KeyCode::ePeriod: return "Period";
		case v3d::KeyCode::eSlash: return "Slash";
		case v3d::KeyCode::e0: return "0";
		case v3d::KeyCode::e1: return "1";
		case v3d::KeyCode::e2: return "2";
		case v3d::KeyCode::e3: return "3";
		case v3d::KeyCode::e4: return "4";
		case v3d::KeyCode::e5: return "5";
		case v3d::KeyCode::e6: return "6";
		case v3d::KeyCode::e7: return "7";
		case v3d::KeyCode::e8: return "8";
		case v3d::KeyCode::e9: return "9";
		case v3d::KeyCode::eSemicolon: return "Semicolon";
		case v3d::KeyCode::eEqual: return "Equal";
		case v3d::KeyCode::eA: return "A";
		case v3d::KeyCode::eB: return "B";
		case v3d::KeyCode::eC: return "C";
		case v3d::KeyCode::eD: return "D";
		case v3d::KeyCode::eE: return "E";
		case v3d::KeyCode::eF: return "F";
		case v3d::KeyCode::eG: return "G";
		case v3d::KeyCode::eH: return "H";
		case v3d::KeyCode::eI: return "I";
		case v3d::KeyCode::eJ: return "J";
		case v3d::KeyCode::eK: return "K";
		case v3d::KeyCode::eL: return "L";
		case v3d::KeyCode::eM: return "M";
		case v3d::KeyCode::eN: return "N";
		case v3d::KeyCode::eO: return "O";
		case v3d::KeyCode::eP: return "P";
		case v3d::KeyCode::eQ: return "Q";
		case v3d::KeyCode::eR: return "R";
		case v3d::KeyCode::eS: return "S";
		case v3d::KeyCode::eT: return "T";
		case v3d::KeyCode::eU: return "U";
		case v3d::KeyCode::eV: return "V";
		case v3d::KeyCode::eW: return "W";
		case v3d::KeyCode::eX: return "X";
		case v3d::KeyCode::eY: return "Y";
		case v3d::KeyCode::eZ: return "Z";
		case v3d::KeyCode::eLeftBracket: return "LeftBracket";
		case v3d::KeyCode::eBackslash: return "Backslash";
		case v3d::KeyCode::eRightBracket: return "RightBracket";
		case v3d::KeyCode::eGraveAccent: return "GraveAccent";
		case v3d::KeyCode::eWorld1: return "World1";
		case v3d::KeyCode::eWorld2: return "World2";
		case v3d::KeyCode::eEscape: return "Escape";
		case v3d::KeyCode::eEnter: return "Enter";
		case v3d::KeyCode::eTab: return "Tab";
		case v3d::KeyCode::eBackspace: return "Backspace";
		case v3d::KeyCode::eInsert: return "Insert";
		case v3d::KeyCode::eDelete: return "Delete";
		case v3d::KeyCode::eRight: return "Right";
		case v3d::KeyCode::eLeft: return "Left";
		case v3d::KeyCode::eDown: return "Down";
		case v3d::KeyCode::eUp: return "Up";
		case v3d::KeyCode::ePageUp: return "PageUp";
		case v3d::KeyCode::ePageDown: return "PageDown";
		case v3d::KeyCode::eHome: return "Home";
		case v3d::KeyCode::eEnd: return "End";
		case v3d::KeyCode::eCapsLock: return "CapsLock";
		case v3d::KeyCode::eScrollLock: return "ScrollLock";
		case v3d::KeyCode::eNumLock: return "NumLock";
		case v3d::KeyCode::ePrintScreen: return "PrintScreen";
		case v3d::KeyCode::ePause: return "Pause";
		case v3d::KeyCode::eF1: return "F1";
		case v3d::KeyCode::eF2: return "F2";
		case v3d::KeyCode::eF3: return "F3";
		case v3d::KeyCode::eF4: return "F4";
		case v3d::KeyCode::eF5: return "F5";
		case v3d::KeyCode::eF6: return "F6";
		case v3d::KeyCode::eF7: return "F7";
		case v3d::KeyCode::eF8: return "F8";
		case v3d::KeyCode::eF9: return "F9";
		case v3d::KeyCode::eF10: return "F10";
		case v3d::KeyCode::eF11: return "F11";
		case v3d::KeyCode::eF12: return "F12";
		case v3d::KeyCode::eF13: return "F13";
		case v3d::KeyCode::eF14: return "F14";
		case v3d::KeyCode::eF15: return "F15";
		case v3d::KeyCode::eF16: return "F16";
		case v3d::KeyCode::eF17: return "F17";
		case v3d::KeyCode::eF18: return "F18";
		case v3d::KeyCode::eF19: return "F19";
		case v3d::KeyCode::eF20: return "F20";
		case v3d::KeyCode::eF21: return "F21";
		case v3d::KeyCode::eF22: return "F22";
		case v3d::KeyCode::eF23: return "F23";
		case v3d::KeyCode::eF24: return "F24";
		case v3d::KeyCode::eF25: return "F25";
		case v3d::KeyCode::eKP0: return "KP0";
		case v3d::KeyCode::eKP1: return "KP1";
		case v3d::KeyCode::eKP2: return "KP2";
		case v3d::KeyCode::eKP3: return "KP3";
		case v3d::KeyCode::eKP4: return "KP4";
		case v3d::KeyCode::eKP5: return "KP5";
		case v3d::KeyCode::eKP6: return "KP6";
		case v3d::KeyCode::eKP7: return "KP7";
		case v3d::KeyCode::eKP8: return "KP8";
		case v3d::KeyCode::eKP9: return "KP9";
		case v3d::KeyCode::eKPDecimal: return "KPDecimal";
		case v3d::KeyCode::eKPDivide: return "KPDivide";
		case v3d::KeyCode::eKPMultiply: return "KPMultiply";
		case v3d::KeyCode::eKPSubtract: return "KPSubtract";
		case v3d::KeyCode::eKPAdd: return "KPAdd";
		case v3d::KeyCode::eKPEnter: return "KPEnter";
		case v3d::KeyCode::eKPEqual: return "KPEqual";
		case v3d::KeyCode::eLeftShift: return "LeftShift";
		case v3d::KeyCode::eLeftControl: return "LeftControl";
		case v3d::KeyCode::eLeftAlt: return "LeftAlt";
		case v3d::KeyCode::eLeftSuper: return "LeftSuper";
		case v3d::KeyCode::eRightShift: return "RightShift";
		case v3d::KeyCode::eRightControl: return "RightControl";
		case v3d::KeyCode::eRightAlt: return "RightAlt";
		case v3d::KeyCode::eRightSuper: return "RightSuper";
		case v3d::KeyCode::eMenu: return "Menu";
		default: return "Invalid key code";
		}
	}

	// Convert v3d::KeyCode to char. Only printable key codes will be converted and returns char of that key. Else, returns 0.
	static char keyCodeToChar(const v3d::KeyCode keyCode, const bool shift)
	{
		switch (keyCode)
		{
		case v3d::KeyCode::eSpace: return ' ';
		case v3d::KeyCode::eApostrophe: return shift ? '"' : '\'';
		case v3d::KeyCode::eComma: return shift ? '<' : ',';
		case v3d::KeyCode::eMinus: return shift ? '_' : '-';
		case v3d::KeyCode::ePeriod: return shift ? '>' : '.';
		case v3d::KeyCode::eSlash: return shift ? '?' : '/';
		case v3d::KeyCode::e0: return shift ? ')' : '0';
		case v3d::KeyCode::e1: return shift ? '!' : '1';
		case v3d::KeyCode::e2: return shift ? '@' : '2';
		case v3d::KeyCode::e3: return shift ? '#' : '3';
		case v3d::KeyCode::e4: return shift ? '$' : '4';
		case v3d::KeyCode::e5: return shift ? '%' : '5';
		case v3d::KeyCode::e6: return shift ? '^' : '6';
		case v3d::KeyCode::e7: return shift ? '&' : '7';
		case v3d::KeyCode::e8: return shift ? '*' : '8';
		case v3d::KeyCode::e9: return shift ? '(' : '9';
		case v3d::KeyCode::eSemicolon: return shift ? ':' : ';';
		case v3d::KeyCode::eEqual: return shift ? '+' : '=';
		case v3d::KeyCode::eA: return shift ? 'A' : 'a';
		case v3d::KeyCode::eB: return shift ? 'B' : 'b';
		case v3d::KeyCode::eC: return shift ? 'C' : 'c';
		case v3d::KeyCode::eD: return shift ? 'D' : 'd';
		case v3d::KeyCode::eE: return shift ? 'E' : 'e';
		case v3d::KeyCode::eF: return shift ? 'F' : 'f';
		case v3d::KeyCode::eG: return shift ? 'G' : 'g';
		case v3d::KeyCode::eH: return shift ? 'H' : 'h';
		case v3d::KeyCode::eI: return shift ? 'I' : 'i';
		case v3d::KeyCode::eJ: return shift ? 'J' : 'j';
		case v3d::KeyCode::eK: return shift ? 'K' : 'k';
		case v3d::KeyCode::eL: return shift ? 'L' : 'l';
		case v3d::KeyCode::eM: return shift ? 'M' : 'm';
		case v3d::KeyCode::eN: return shift ? 'N' : 'n';
		case v3d::KeyCode::eO: return shift ? 'O' : 'o';
		case v3d::KeyCode::eP: return shift ? 'P' : 'p';
		case v3d::KeyCode::eQ: return shift ? 'Q' : 'q';
		case v3d::KeyCode::eR: return shift ? 'R' : 'r';
		case v3d::KeyCode::eS: return shift ? 'S' : 's';
		case v3d::KeyCode::eT: return shift ? 'T' : 't';
		case v3d::KeyCode::eU: return shift ? 'U' : 'u';
		case v3d::KeyCode::eV: return shift ? 'V' : 'v';
		case v3d::KeyCode::eW: return shift ? 'W' : 'w';
		case v3d::KeyCode::eX: return shift ? 'X' : 'x';
		case v3d::KeyCode::eY: return shift ? 'Y' : 'y';
		case v3d::KeyCode::eZ: return shift ? 'Z' : 'z';
		case v3d::KeyCode::eLeftBracket: return shift ? '{' : '[';
		case v3d::KeyCode::eBackslash: return shift ? '|' : '\\';
		case v3d::KeyCode::eRightBracket: return shift ? '}' : ']';
		case v3d::KeyCode::eGraveAccent: return shift ? '~' : '`';
		case v3d::KeyCode::eKP0: return '0';
		case v3d::KeyCode::eKP1: return '1';
		case v3d::KeyCode::eKP2: return '2';
		case v3d::KeyCode::eKP3: return '3';
		case v3d::KeyCode::eKP4: return '4';
		case v3d::KeyCode::eKP5: return '5';
		case v3d::KeyCode::eKP6: return '6';
		case v3d::KeyCode::eKP7: return '7';
		case v3d::KeyCode::eKP8: return '8';
		case v3d::KeyCode::eKP9: return '9';
		case v3d::KeyCode::eKPDivide: return '/';
		case v3d::KeyCode::eKPMultiply: return '*';
		case v3d::KeyCode::eKPSubtract: return '-';
		case v3d::KeyCode::eKPAdd: return '+';
		case v3d::KeyCode::eWorld1:
		case v3d::KeyCode::eWorld2:
		case v3d::KeyCode::eEscape:
		case v3d::KeyCode::eEnter:
		case v3d::KeyCode::eTab:
		case v3d::KeyCode::eBackspace:
		case v3d::KeyCode::eInsert:
		case v3d::KeyCode::eDelete:
		case v3d::KeyCode::eRight:
		case v3d::KeyCode::eLeft:
		case v3d::KeyCode::eDown:
		case v3d::KeyCode::eUp:
		case v3d::KeyCode::ePageUp:
		case v3d::KeyCode::ePageDown:
		case v3d::KeyCode::eHome:
		case v3d::KeyCode::eEnd:
		case v3d::KeyCode::eCapsLock:
		case v3d::KeyCode::eScrollLock:
		case v3d::KeyCode::eNumLock:
		case v3d::KeyCode::ePrintScreen:
		case v3d::KeyCode::ePause:
		case v3d::KeyCode::eF1:
		case v3d::KeyCode::eF2:
		case v3d::KeyCode::eF3:
		case v3d::KeyCode::eF4:
		case v3d::KeyCode::eF5:
		case v3d::KeyCode::eF6:
		case v3d::KeyCode::eF7:
		case v3d::KeyCode::eF8:
		case v3d::KeyCode::eF9:
		case v3d::KeyCode::eF10:
		case v3d::KeyCode::eF11:
		case v3d::KeyCode::eF12:
		case v3d::KeyCode::eF13:
		case v3d::KeyCode::eF14:
		case v3d::KeyCode::eF15:
		case v3d::KeyCode::eF16:
		case v3d::KeyCode::eF17:
		case v3d::KeyCode::eF18:
		case v3d::KeyCode::eF19:
		case v3d::KeyCode::eF20:
		case v3d::KeyCode::eF21:
		case v3d::KeyCode::eF22:
		case v3d::KeyCode::eF23:
		case v3d::KeyCode::eF24:
		case v3d::KeyCode::eF25:
		case v3d::KeyCode::eKPDecimal:
		case v3d::KeyCode::eKPEnter:
		case v3d::KeyCode::eKPEqual:
		case v3d::KeyCode::eLeftShift:
		case v3d::KeyCode::eLeftControl:
		case v3d::KeyCode::eLeftAlt:
		case v3d::KeyCode::eLeftSuper:
		case v3d::KeyCode::eRightShift:
		case v3d::KeyCode::eRightControl:
		case v3d::KeyCode::eRightAlt:
		case v3d::KeyCode::eRightSuper:
		case v3d::KeyCode::eMenu:
		case v3d::KeyCode::eEnumLast:
		default: return 0;
		}
	}
}

#endif