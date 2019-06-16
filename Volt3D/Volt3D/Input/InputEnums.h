#ifndef V3D_INPUT_ENUMS_H
#define V3D_INPUT_ENUMS_H

#include <string>

namespace v3d
{
	/**
	*	@enum InputStae
	*	State of input for keyboard and mouse.
	*/
	enum class InputState : unsigned int
	{
		INPUT_PRESSED = 0,
		INPUT_RELEASED,
	};

	/**
	*	@enum KeyCode
	*	A list of key code that remaps GLFW's key define.
	*/
	enum class KeyCode : int
	{
		// Unknown key
		//KEY_UNKNOWN = -1,
		// Printable keys
		KEY_SPACE = 0,
		KEY_APOSTROPHE,				/* ' */
		KEY_COMMA,					/* , */
		KEY_MINUS,					/* - */
		KEY_PERIOD,					/* . */
		KEY_SLASH,					/* / */
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_SEMICOLON,				/* ; */
		KEY_EQUAL,					/* = */
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_LEFT_BRACKET,		/* [ */
		KEY_BACKSLASH,			/* \ */
		KEY_RIGHT_BRACKET,		/* ] */
		KEY_GRAVE_ACCENT,		/* ` */
		KEY_WORLD_1,			/* non-US #1 */
		KEY_WORLD_2,			/* non-US #2 */
		// Function keys
		KEY_ESCAPE,
		KEY_ENTER,
		KEY_TAB,
		KEY_BACKSPACE,
		KEY_INSERT,
		KEY_DELETE,
		KEY_RIGHT,
		KEY_LEFT,
		KEY_DOWN,
		KEY_UP,
		KEY_PAGE_UP,
		KEY_PAGE_DOWN,
		KEY_HOME,
		KEY_END,
		KEY_CAPS_LOCK,
		KEY_SCROLL_LOCK,
		KEY_NUM_LOCK,
		KEY_PRINT_SCREEN,
		KEY_PAUSE,
		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,
		KEY_F13,
		KEY_F14,
		KEY_F15,
		KEY_F16,
		KEY_F17,
		KEY_F18,
		KEY_F19,
		KEY_F20,
		KEY_F21,
		KEY_F22,
		KEY_F23,
		KEY_F24,
		KEY_F25,
		KEY_KP_0,
		KEY_KP_1,
		KEY_KP_2,
		KEY_KP_3,
		KEY_KP_4,
		KEY_KP_5,
		KEY_KP_6,
		KEY_KP_7,
		KEY_KP_8,
		KEY_KP_9,
		KEY_KP_DECIMAL,
		KEY_KP_DIVIDE,
		KEY_KP_MULTIPLY,
		KEY_KP_SUBTRACT,
		KEY_KP_ADD,
		KEY_KP_ENTER,
		KEY_KP_EQUAL,
		KEY_LEFT_SHIFT,
		KEY_LEFT_CONTROL,
		KEY_LEFT_ALT,
		KEY_LEFT_SUPER,			/* Left OS key */
		KEY_RIGHT_SHIFT,
		KEY_RIGHT_CONTROL,
		KEY_RIGHT_ALT,
		KEY_RIGHT_SUPER,		/* Right OS key */
		KEY_MENU,
		// Last
		KEY_ENUM_LAST,
	};

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
	enum KeyModifier : unsigned int
	{
		KEY_MOD_NONE =		0,				// 0
		KEY_MOD_SHIFT =		1 << 0,			// 1
		KEY_MOD_CONTROL =	1 << 1,			// 2
		KEY_MOD_ALT =		1 << 2,			// 4
		KEY_MOD_SUPER =		1 << 3,			// 8
		KEY_MOD_CAPS_LOCK = 1 << 4,			// 16
		KEY_MOD_NUM_LOCK =	1 << 5,			// 32
	};

	/**
	*	@enum MouseButton
	*	A list of mouse buttons that remaps GLFW's mouse button define
	*/
	enum class MouseButton : unsigned int
	{
		MOUSE_BUTTON_1 = 0,
		MOUSE_BUTTON_2,
		MOUSE_BUTTON_3,
		MOUSE_BUTTON_4,
		MOUSE_BUTTON_5,
		MOUSE_BUTTON_6,
		MOUSE_BUTTON_7,
		MOUSE_BUTTON_8,
		MOUSE_BUTTON_ENUM_LAST,
		MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1,
		MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2,
		MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3,
	};

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

	// Convert v3d::KeyCode to string name.
	static std::string keyCodeToStr(const v3d::KeyCode keycode)
	{
		std::string str;

		switch (keycode)
		{
		case v3d::KeyCode::KEY_SPACE:
			str = "SPACE";
			break;
		case v3d::KeyCode::KEY_APOSTROPHE:
			str = "'";
			break;
		case v3d::KeyCode::KEY_COMMA:
			str = ",";
			break;
		case v3d::KeyCode::KEY_MINUS:
			str = "-";
			break;
		case v3d::KeyCode::KEY_PERIOD:
			str = ".";
			break;
		case v3d::KeyCode::KEY_SLASH:
			str = "/";
			break;
		case v3d::KeyCode::KEY_0:
			str = "0";
			break;
		case v3d::KeyCode::KEY_1:
			str = "1";
			break;
		case v3d::KeyCode::KEY_2:
			str = "2";
			break;
		case v3d::KeyCode::KEY_3:
			str = "3";
			break;
		case v3d::KeyCode::KEY_4:
			str = "4";
			break;
		case v3d::KeyCode::KEY_5:
			str = "5";
			break;
		case v3d::KeyCode::KEY_6:
			str = "6";
			break;
		case v3d::KeyCode::KEY_7:
			str = "7";
			break;
		case v3d::KeyCode::KEY_8:
			str = "8";
			break;
		case v3d::KeyCode::KEY_9:
			str = "9";
			break;
		case v3d::KeyCode::KEY_SEMICOLON:
			str = ";";
			break;
		case v3d::KeyCode::KEY_EQUAL:
			str = "=";
			break;
		case v3d::KeyCode::KEY_A:
			str = "A";
			break;
		case v3d::KeyCode::KEY_B:
			str = "B";
			break;
		case v3d::KeyCode::KEY_C:
			str = "C";
			break;
		case v3d::KeyCode::KEY_D:
			str = "D";
			break;
		case v3d::KeyCode::KEY_E:
			str = "E";
			break;
		case v3d::KeyCode::KEY_F:
			str = "F";
			break;
		case v3d::KeyCode::KEY_G:
			str = "G";
			break;
		case v3d::KeyCode::KEY_H:
			str = "H";
			break;
		case v3d::KeyCode::KEY_I:
			str = "I";
			break;
		case v3d::KeyCode::KEY_J:
			str = "J";
			break;
		case v3d::KeyCode::KEY_K:
			str = "K";
			break;
		case v3d::KeyCode::KEY_L:
			str = "L";
			break;
		case v3d::KeyCode::KEY_M:
			str = "M";
			break;
		case v3d::KeyCode::KEY_N:
			str = "N";
			break;
		case v3d::KeyCode::KEY_O:
			str = "O";
			break;
		case v3d::KeyCode::KEY_P:
			str = "P";
			break;
		case v3d::KeyCode::KEY_Q:
			str = "Q";
			break;
		case v3d::KeyCode::KEY_R:
			str = "R";
			break;
		case v3d::KeyCode::KEY_S:
			str = "S";
			break;
		case v3d::KeyCode::KEY_T:
			str = "T";
			break;
		case v3d::KeyCode::KEY_U:
			str = "U";
			break;
		case v3d::KeyCode::KEY_V:
			str = "V";
			break;
		case v3d::KeyCode::KEY_W:
			str = "W";
			break;
		case v3d::KeyCode::KEY_X:
			str = "X";
			break;
		case v3d::KeyCode::KEY_Y:
			str = "Y";
			break;
		case v3d::KeyCode::KEY_Z:
			str = "Z";
			break;
		case v3d::KeyCode::KEY_LEFT_BRACKET:
			str = "[";
			break;
		case v3d::KeyCode::KEY_BACKSLASH:
			str = "\\";
			break;
		case v3d::KeyCode::KEY_RIGHT_BRACKET:
			str = "]";
			break;
		case v3d::KeyCode::KEY_GRAVE_ACCENT:
			str = "`";
			break;
		case v3d::KeyCode::KEY_WORLD_1:
			str = "WORLD 1";
			break;
		case v3d::KeyCode::KEY_WORLD_2:
			str = "WORLD 2";
			break;
		case v3d::KeyCode::KEY_ESCAPE:
			str = "ESCAPE";
			break;
		case v3d::KeyCode::KEY_ENTER:
			str = "ENTER";
			break;
		case v3d::KeyCode::KEY_TAB:
			str = "TAB";
			break;
		case v3d::KeyCode::KEY_BACKSPACE:
			str = "BACKSPACE";
			break;
		case v3d::KeyCode::KEY_INSERT:
			str = "INSERT";
			break;
		case v3d::KeyCode::KEY_DELETE:
			str = "DELETE";
			break;
		case v3d::KeyCode::KEY_RIGHT:
			str = "RIGHT";
			break;
		case v3d::KeyCode::KEY_LEFT:
			str = "LEFT";
			break;
		case v3d::KeyCode::KEY_DOWN:
			str = "DOWN";
			break;
		case v3d::KeyCode::KEY_UP:
			str = "UP";
			break;
		case v3d::KeyCode::KEY_PAGE_UP:
			str = "PAGE UP";
			break;
		case v3d::KeyCode::KEY_PAGE_DOWN:
			str = "PAGE DOWN";
			break;
		case v3d::KeyCode::KEY_HOME:
			str = "HOME";
			break;
		case v3d::KeyCode::KEY_END:
			str = "END";
			break;
		case v3d::KeyCode::KEY_CAPS_LOCK:
			str = "CAPS LOCK";
			break;
		case v3d::KeyCode::KEY_SCROLL_LOCK:
			str = "SCROLL LOCK";
			break;
		case v3d::KeyCode::KEY_NUM_LOCK:
			str = "NUM LOCK";
			break;
		case v3d::KeyCode::KEY_PRINT_SCREEN:
			str = "PRINT SCREEN";
			break;
		case v3d::KeyCode::KEY_PAUSE:
			str = "PAUSE";
			break;
		case v3d::KeyCode::KEY_F1:
			str = "F1";
			break;
		case v3d::KeyCode::KEY_F2:
			str = "F2";
			break;
		case v3d::KeyCode::KEY_F3:
			str = "F3";
			break;
		case v3d::KeyCode::KEY_F4:
			str = "F4";
			break;
		case v3d::KeyCode::KEY_F5:
			str = "F5";
			break;
		case v3d::KeyCode::KEY_F6:
			str = "F6";
			break;
		case v3d::KeyCode::KEY_F7:
			str = "F7";
			break;
		case v3d::KeyCode::KEY_F8:
			str = "F8";
			break;
		case v3d::KeyCode::KEY_F9:
			str = "F9";
			break;
		case v3d::KeyCode::KEY_F10:
			str = "F10";
			break;
		case v3d::KeyCode::KEY_F11:
			str = "F11";
			break;
		case v3d::KeyCode::KEY_F12:
			str = "F12";
			break;
		case v3d::KeyCode::KEY_F13:
			str = "F13";
			break;
		case v3d::KeyCode::KEY_F14:
			str = "F14";
			break;
		case v3d::KeyCode::KEY_F15:
			str = "F15";
			break;
		case v3d::KeyCode::KEY_F16:
			str = "F16";
			break;
		case v3d::KeyCode::KEY_F17:
			str = "F17";
			break;
		case v3d::KeyCode::KEY_F18:
			str = "F18";
			break;
		case v3d::KeyCode::KEY_F19:
			str = "F19";
			break;
		case v3d::KeyCode::KEY_F20:
			str = "F20";
			break;
		case v3d::KeyCode::KEY_F21:
			str = "F21";
			break;
		case v3d::KeyCode::KEY_F22:
			str = "F22";
			break;
		case v3d::KeyCode::KEY_F23:
			str = "F23";
			break;
		case v3d::KeyCode::KEY_F24:
			str = "F24";
			break;
		case v3d::KeyCode::KEY_F25:
			str = "F25";
			break;
		case v3d::KeyCode::KEY_KP_0:
			str = "Keypad 0";
			break;
		case v3d::KeyCode::KEY_KP_1:
			str = "Keypad 1";
			break;
		case v3d::KeyCode::KEY_KP_2:
			str = "Keypad 2";
			break;
		case v3d::KeyCode::KEY_KP_3:
			str = "Keypad 3";
			break;
		case v3d::KeyCode::KEY_KP_4:
			str = "Keypad 4";
			break;
		case v3d::KeyCode::KEY_KP_5:
			str = "Keypad 5";
			break;
		case v3d::KeyCode::KEY_KP_6:
			str = "Keypad 6";
			break;
		case v3d::KeyCode::KEY_KP_7:
			str = "Keypad 7";
			break;
		case v3d::KeyCode::KEY_KP_8:
			str = "Keypad 8";
			break;
		case v3d::KeyCode::KEY_KP_9:
			str = "Keypad 9";
			break;
		case v3d::KeyCode::KEY_KP_DECIMAL:
			str = "Keypad .";
			break;
		case v3d::KeyCode::KEY_KP_DIVIDE:
			str = "Keypad /";
			break;
		case v3d::KeyCode::KEY_KP_MULTIPLY:
			str = "Keypad *";
			break;
		case v3d::KeyCode::KEY_KP_SUBTRACT:
			str = "Keypad -";
			break;
		case v3d::KeyCode::KEY_KP_ADD:
			str = "Keypad +";
			break;
		case v3d::KeyCode::KEY_KP_ENTER:
			str = "Keypad ENTER";
			break;
		case v3d::KeyCode::KEY_KP_EQUAL:
			str = "Keypad =";
			break;
		case v3d::KeyCode::KEY_LEFT_SHIFT:
			str = "LEFT SHIFT";
			break;
		case v3d::KeyCode::KEY_LEFT_CONTROL:
			str = "LEFT CONTROL";
			break;
		case v3d::KeyCode::KEY_LEFT_ALT:
			str = "LEFT ALT";
			break;
		case v3d::KeyCode::KEY_LEFT_SUPER:
			str = "LEFT SUPER";
			break;
		case v3d::KeyCode::KEY_RIGHT_SHIFT:
			str = "RIGHT SHIFT";
			break;
		case v3d::KeyCode::KEY_RIGHT_CONTROL:
			str = "RIGHT CONTROL";
			break;
		case v3d::KeyCode::KEY_RIGHT_ALT:
			str = "RIGHT ALT";
			break;
		case v3d::KeyCode::KEY_RIGHT_SUPER:
			str = "RIGHT SUPER";
			break;
		case v3d::KeyCode::KEY_MENU:
			str = "MENU";
			break;
		default:
			break;
		}

		return str;
	}

	// Convert v3d::KeyCode to char. Only printable key codes will be converted and returns char of that key. Else, returns 0.
	static char keyCodeToChar(const v3d::KeyCode keyCode, const bool shift)
	{
		char c;

		switch (keyCode)
		{
		case v3d::KeyCode::KEY_SPACE:
			c = ' ';
			break;
		case v3d::KeyCode::KEY_APOSTROPHE:
			if (shift) c = '"';
			else c = '\'';
			break;
		case v3d::KeyCode::KEY_COMMA:
			if (shift) c = '<';
			else c = ',';
			break;
		case v3d::KeyCode::KEY_MINUS:
			if (shift) c = '_';
			else c = '-';
			break;
		case v3d::KeyCode::KEY_PERIOD:
			if (shift) c = '>';
			else c = '.';
			break;
		case v3d::KeyCode::KEY_SLASH:
			if (shift) c = '?';
			else c = '/';
			break;
		case v3d::KeyCode::KEY_0:
			if (shift) c = ')';
			else c = '0';
			break;
		case v3d::KeyCode::KEY_1:
			if (shift) c = '!';
			else c = '1';
			break;
		case v3d::KeyCode::KEY_2:
			if (shift) c = '@';
			else c = '2';
			break;
		case v3d::KeyCode::KEY_3:
			if (shift) c = '#';
			else c = '3';
			break;
		case v3d::KeyCode::KEY_4:
			if (shift) c = '$';
			else c = '4';
			break;
		case v3d::KeyCode::KEY_5:
			if (shift) c = '%';
			else c = '5';
			break;
		case v3d::KeyCode::KEY_6:
			if (shift) c = '^';
			else c = '6';
			break;
		case v3d::KeyCode::KEY_7:
			if (shift) c = '&';
			else c = '7';
			break;
		case v3d::KeyCode::KEY_8:
			if (shift) c = '*';
			else c = '8';
			break;
		case v3d::KeyCode::KEY_9:
			if (shift) c = '(';
			else c = '9';
			break;
		case v3d::KeyCode::KEY_SEMICOLON:
			if (shift) c = ':';
			else c = ';';
			break;
		case v3d::KeyCode::KEY_EQUAL:
			if (shift) c = '+';
			else c = '=';
			break;
		case v3d::KeyCode::KEY_A:
			if (shift) c = 'A';
			else c = 'a';
			break;
		case v3d::KeyCode::KEY_B:
			if (shift) c = 'B';
			else c = 'b';
			break;
		case v3d::KeyCode::KEY_C:
			if (shift) c = 'C';
			else c = 'c';
			break;
		case v3d::KeyCode::KEY_D:
			if (shift) c = 'D';
			else c = 'd';
			break;
		case v3d::KeyCode::KEY_E:
			if (shift) c = 'E';
			else c = 'e';
			break;
		case v3d::KeyCode::KEY_F:
			if (shift) c = 'F';
			else c = 'f';
			break;
		case v3d::KeyCode::KEY_G:
			if (shift) c = 'G';
			else c = 'g';
			break;
		case v3d::KeyCode::KEY_H:
			if (shift) c = 'H';
			else c = 'h';
			break;
		case v3d::KeyCode::KEY_I:
			if (shift) c = 'I';
			else c = 'i';
			break;
		case v3d::KeyCode::KEY_J:
			if (shift) c = 'J';
			else c = 'j';
			break;
		case v3d::KeyCode::KEY_K:
			if (shift) c = 'K';
			else c = 'k';
			break;
		case v3d::KeyCode::KEY_L:
			if (shift) c = 'L';
			else c = 'l';
			break;
		case v3d::KeyCode::KEY_M:
			if (shift) c = 'M';
			else c = 'm';
			break;
		case v3d::KeyCode::KEY_N:
			if (shift) c = 'N';
			else c = 'n';
			break;
		case v3d::KeyCode::KEY_O:
			if (shift) c = 'O';
			else c = 'o';
			break;
		case v3d::KeyCode::KEY_P:
			if (shift) c = 'P';
			else c = 'p';
			break;
		case v3d::KeyCode::KEY_Q:
			if (shift) c = 'Q';
			else c = 'q';
			break;
		case v3d::KeyCode::KEY_R:
			if (shift) c = 'R';
			else c = 'r';
			break;
		case v3d::KeyCode::KEY_S:
			if (shift) c = 'S';
			else c = 's';
			break;
		case v3d::KeyCode::KEY_T:
			if (shift) c = 'T';
			else c = 't';
			break;
		case v3d::KeyCode::KEY_U:
			if (shift) c = 'U';
			else c = 'u';
			break;
		case v3d::KeyCode::KEY_V:
			if (shift) c = 'V';
			else c = 'v';
			break;
		case v3d::KeyCode::KEY_W:
			if (shift) c = 'W';
			else c = 'w';
			break;
		case v3d::KeyCode::KEY_X:
			if (shift) c = 'X';
			else c = 'x';
			break;
		case v3d::KeyCode::KEY_Y:
			if (shift) c = 'Y';
			else c = 'y';
			break;
		case v3d::KeyCode::KEY_Z:
			if (shift) c = 'Z';
			else c = 'z';
			break;
		case v3d::KeyCode::KEY_LEFT_BRACKET:
			if (shift) c = '{';
			else c = '[';
			break;
		case v3d::KeyCode::KEY_BACKSLASH:
			if (shift) c = '|';
			else c = '\\';
			break;
		case v3d::KeyCode::KEY_RIGHT_BRACKET:
			if (shift) c = '}';
			else c = ']';
			break;
		case v3d::KeyCode::KEY_GRAVE_ACCENT:
			if (shift) c = '~';
			else c = '`';
			break;
		case v3d::KeyCode::KEY_KP_0:
			c = '0';
			break;
		case v3d::KeyCode::KEY_KP_1:
			c = '1';
			break;
		case v3d::KeyCode::KEY_KP_2:
			c = '2';
			break;
		case v3d::KeyCode::KEY_KP_3:
			c = '3';
			break;
		case v3d::KeyCode::KEY_KP_4:
			c = '4';
			break;
		case v3d::KeyCode::KEY_KP_5:
			c = '5';
			break;
		case v3d::KeyCode::KEY_KP_6:
			c = '6';
			break;
		case v3d::KeyCode::KEY_KP_7:
			c = '7';
			break;
		case v3d::KeyCode::KEY_KP_8:
			c = '8';
			break;
		case v3d::KeyCode::KEY_KP_9:
			c = '9';
			break;
		case v3d::KeyCode::KEY_KP_DIVIDE:
			c = '/';
			break;
		case v3d::KeyCode::KEY_KP_MULTIPLY:
			c = '*';
			break;
		case v3d::KeyCode::KEY_KP_SUBTRACT:
			c = '-';
			break;
		case v3d::KeyCode::KEY_KP_ADD:
			c = '+';
			break;
		case v3d::KeyCode::KEY_WORLD_1:
		case v3d::KeyCode::KEY_WORLD_2:
		case v3d::KeyCode::KEY_ESCAPE:
		case v3d::KeyCode::KEY_ENTER:
		case v3d::KeyCode::KEY_TAB:
		case v3d::KeyCode::KEY_BACKSPACE:
		case v3d::KeyCode::KEY_INSERT:
		case v3d::KeyCode::KEY_DELETE:
		case v3d::KeyCode::KEY_RIGHT:
		case v3d::KeyCode::KEY_LEFT:
		case v3d::KeyCode::KEY_DOWN:
		case v3d::KeyCode::KEY_UP:
		case v3d::KeyCode::KEY_PAGE_UP:
		case v3d::KeyCode::KEY_PAGE_DOWN:
		case v3d::KeyCode::KEY_HOME:
		case v3d::KeyCode::KEY_END:
		case v3d::KeyCode::KEY_CAPS_LOCK:
		case v3d::KeyCode::KEY_SCROLL_LOCK:
		case v3d::KeyCode::KEY_NUM_LOCK:
		case v3d::KeyCode::KEY_PRINT_SCREEN:
		case v3d::KeyCode::KEY_PAUSE:
		case v3d::KeyCode::KEY_F1:
		case v3d::KeyCode::KEY_F2:
		case v3d::KeyCode::KEY_F3:
		case v3d::KeyCode::KEY_F4:
		case v3d::KeyCode::KEY_F5:
		case v3d::KeyCode::KEY_F6:
		case v3d::KeyCode::KEY_F7:
		case v3d::KeyCode::KEY_F8:
		case v3d::KeyCode::KEY_F9:
		case v3d::KeyCode::KEY_F10:
		case v3d::KeyCode::KEY_F11:
		case v3d::KeyCode::KEY_F12:
		case v3d::KeyCode::KEY_F13:
		case v3d::KeyCode::KEY_F14:
		case v3d::KeyCode::KEY_F15:
		case v3d::KeyCode::KEY_F16:
		case v3d::KeyCode::KEY_F17:
		case v3d::KeyCode::KEY_F18:
		case v3d::KeyCode::KEY_F19:
		case v3d::KeyCode::KEY_F20:
		case v3d::KeyCode::KEY_F21:
		case v3d::KeyCode::KEY_F22:
		case v3d::KeyCode::KEY_F23:
		case v3d::KeyCode::KEY_F24:
		case v3d::KeyCode::KEY_F25:
		case v3d::KeyCode::KEY_KP_DECIMAL:
		case v3d::KeyCode::KEY_KP_ENTER:
		case v3d::KeyCode::KEY_KP_EQUAL:
		case v3d::KeyCode::KEY_LEFT_SHIFT:
		case v3d::KeyCode::KEY_LEFT_CONTROL:
		case v3d::KeyCode::KEY_LEFT_ALT:
		case v3d::KeyCode::KEY_LEFT_SUPER:
		case v3d::KeyCode::KEY_RIGHT_SHIFT:
		case v3d::KeyCode::KEY_RIGHT_CONTROL:
		case v3d::KeyCode::KEY_RIGHT_ALT:
		case v3d::KeyCode::KEY_RIGHT_SUPER:
		case v3d::KeyCode::KEY_MENU:
		case v3d::KeyCode::KEY_ENUM_LAST:
		default:
			c = 0;
			break;
		}

		return c;
	}
}

#endif