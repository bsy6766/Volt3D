#include <PreCompiled.h>

#include "InputManager.h"

#include <GLFW/glfw3.h>

//#include "GameController.h"

#include "Engine/Engine.h"
//#include "Engine/GLView.h"
#include "Utils/Logger.h"

#include "Node/TransformNode.h"

//#include "UI/TextField.h"

//#include "Error/ErrorCodes.h"
//#include "Error/ErrorReport.h"

#include "Config/BuildConfig.h"
//#include "Config/Config.h"
//#include "Config/LogConfig.h"

V3D_NS_BEGIN

// GLFW key define to v3d::KeyCode
std::unordered_map<int, v3d::KeyCode> InputManager::glfwToKeyCodeMap = 
{
	//{ GLFW_KEY_UNKNOWN, v3d::KeyCode::eUnknown },				// -1
	{ GLFW_KEY_SPACE, v3d::KeyCode::eSpace },					// 32
	{ GLFW_KEY_APOSTROPHE, v3d::KeyCode::eApostrophe },			// 39  /* ' */
	{ GLFW_KEY_COMMA, v3d::KeyCode::eComma },					// 44  /* , */
	{ GLFW_KEY_MINUS, v3d::KeyCode::eMinus },					// 45  /* - */
	{ GLFW_KEY_PERIOD, v3d::KeyCode::ePeriod },					// 46  /* . */
	{ GLFW_KEY_SLASH, v3d::KeyCode::eSlash },					// 47  /* / */
	{ GLFW_KEY_0, v3d::KeyCode::e0 },							// 48
	{ GLFW_KEY_1, v3d::KeyCode::e1 },							// 49
	{ GLFW_KEY_2, v3d::KeyCode::e2 },							// 50
	{ GLFW_KEY_3, v3d::KeyCode::e3 },							// 51
	{ GLFW_KEY_4, v3d::KeyCode::e4 },							// 52
	{ GLFW_KEY_5, v3d::KeyCode::e5 },							// 53
	{ GLFW_KEY_6, v3d::KeyCode::e6 },							// 54
	{ GLFW_KEY_7, v3d::KeyCode::e7 },							// 55
	{ GLFW_KEY_8, v3d::KeyCode::e8 },							// 56
	{ GLFW_KEY_9, v3d::KeyCode::e9 },							// 57
	{ GLFW_KEY_SEMICOLON, v3d::KeyCode::eSemicolon },			// 59  /* ; */
	{ GLFW_KEY_EQUAL, v3d::KeyCode::eEqual },					// 61  /* = */
	{ GLFW_KEY_A, v3d::KeyCode::eA },							// 65
	{ GLFW_KEY_B, v3d::KeyCode::eB },							// 66
	{ GLFW_KEY_C, v3d::KeyCode::eC },							// 67
	{ GLFW_KEY_D, v3d::KeyCode::eD },							// 68
	{ GLFW_KEY_E, v3d::KeyCode::eE },							// 69
	{ GLFW_KEY_F, v3d::KeyCode::eF },							// 70
	{ GLFW_KEY_G, v3d::KeyCode::eG },							// 71
	{ GLFW_KEY_H, v3d::KeyCode::eH },							// 72
	{ GLFW_KEY_I, v3d::KeyCode::eI },							// 73
	{ GLFW_KEY_J, v3d::KeyCode::eJ },							// 74
	{ GLFW_KEY_K, v3d::KeyCode::eK },							// 75
	{ GLFW_KEY_L, v3d::KeyCode::eL },							// 76
	{ GLFW_KEY_M, v3d::KeyCode::eM },							// 77
	{ GLFW_KEY_N, v3d::KeyCode::eN },							// 78
	{ GLFW_KEY_O, v3d::KeyCode::eO },							// 79
	{ GLFW_KEY_P, v3d::KeyCode::eP },							// 80
	{ GLFW_KEY_Q, v3d::KeyCode::eQ },							// 81
	{ GLFW_KEY_R, v3d::KeyCode::eR },							// 82
	{ GLFW_KEY_S, v3d::KeyCode::eS },							// 83
	{ GLFW_KEY_T, v3d::KeyCode::eT },							// 84
	{ GLFW_KEY_U, v3d::KeyCode::eU },							// 85
	{ GLFW_KEY_V, v3d::KeyCode::eV },							// 86
	{ GLFW_KEY_W, v3d::KeyCode::eW },							// 87
	{ GLFW_KEY_X, v3d::KeyCode::eX },							// 88
	{ GLFW_KEY_Y, v3d::KeyCode::eY },							// 89
	{ GLFW_KEY_Z, v3d::KeyCode::eZ },							// 90
	{ GLFW_KEY_LEFT_BRACKET, v3d::KeyCode::eLeftBracket },		// 91  /* [ */
	{ GLFW_KEY_BACKSLASH, v3d::KeyCode::eBackslash },			// 92  /* \ */
	{ GLFW_KEY_RIGHT_BRACKET, v3d::KeyCode::eRightBracket },	// 93  /* ] */
	{ GLFW_KEY_GRAVE_ACCENT, v3d::KeyCode::eGraveAccent },		// 96  /* ` */
	{ GLFW_KEY_WORLD_1, v3d::KeyCode::eWorld1 },				// 161 /* non-US #1 */
	{ GLFW_KEY_WORLD_2, v3d::KeyCode::eWorld2 },				// 162 /* non-US #2 */
	{ GLFW_KEY_ESCAPE, v3d::KeyCode::eEscape },					// 256
	{ GLFW_KEY_ENTER, v3d::KeyCode::eEnter },					// 257
	{ GLFW_KEY_TAB, v3d::KeyCode::eTab },						// 258
	{ GLFW_KEY_BACKSPACE, v3d::KeyCode::eBackspace },			// 259
	{ GLFW_KEY_INSERT, v3d::KeyCode::eInsert },					// 260
	{ GLFW_KEY_DELETE, v3d::KeyCode::eDelete },					// 261
	{ GLFW_KEY_RIGHT, v3d::KeyCode::eRight },					// 262
	{ GLFW_KEY_LEFT, v3d::KeyCode::eLeft },						// 263
	{ GLFW_KEY_DOWN, v3d::KeyCode::eDown },						// 264
	{ GLFW_KEY_UP, v3d::KeyCode::eUp },							// 265
	{ GLFW_KEY_PAGE_UP, v3d::KeyCode::ePageUp },				// 266
	{ GLFW_KEY_PAGE_DOWN, v3d::KeyCode::ePageDown },			// 267
	{ GLFW_KEY_HOME, v3d::KeyCode::eHome },						// 268
	{ GLFW_KEY_END, v3d::KeyCode::eEnd },						// 269
	{ GLFW_KEY_CAPS_LOCK, v3d::KeyCode::eCapsLock },			// 280
	{ GLFW_KEY_SCROLL_LOCK, v3d::KeyCode::eScrollLock },		// 281
	{ GLFW_KEY_NUM_LOCK, v3d::KeyCode::eNumLock },				// 282
	{ GLFW_KEY_PRINT_SCREEN, v3d::KeyCode::ePrintScreen },		// 283
	{ GLFW_KEY_PAUSE, v3d::KeyCode::ePause },					// 284
	{ GLFW_KEY_F1, v3d::KeyCode::eF1 },							// 290
	{ GLFW_KEY_F2, v3d::KeyCode::eF2 },							// 291
	{ GLFW_KEY_F3, v3d::KeyCode::eF3 },							// 292
	{ GLFW_KEY_F4, v3d::KeyCode::eF4 },							// 293
	{ GLFW_KEY_F5, v3d::KeyCode::eF5 },							// 294
	{ GLFW_KEY_F6, v3d::KeyCode::eF6 },							// 295
	{ GLFW_KEY_F7, v3d::KeyCode::eF7 },							// 296
	{ GLFW_KEY_F8, v3d::KeyCode::eF8 },							// 297
	{ GLFW_KEY_F9, v3d::KeyCode::eF9 },							// 298
	{ GLFW_KEY_F10, v3d::KeyCode::eF10 },						// 299
	{ GLFW_KEY_F11, v3d::KeyCode::eF11 },						// 300
	{ GLFW_KEY_F12, v3d::KeyCode::eF12 },						// 301
	{ GLFW_KEY_F13, v3d::KeyCode::eF13 },						// 302
	{ GLFW_KEY_F14, v3d::KeyCode::eF14 },						// 303
	{ GLFW_KEY_F15, v3d::KeyCode::eF15 },						// 304
	{ GLFW_KEY_F16, v3d::KeyCode::eF16 },						// 305
	{ GLFW_KEY_F17, v3d::KeyCode::eF17 },						// 306
	{ GLFW_KEY_F18, v3d::KeyCode::eF18 },						// 307
	{ GLFW_KEY_F19, v3d::KeyCode::eF19 },						// 308
	{ GLFW_KEY_F20, v3d::KeyCode::eF20 },						// 309
	{ GLFW_KEY_F21, v3d::KeyCode::eF21 },						// 310
	{ GLFW_KEY_F22, v3d::KeyCode::eF22 },						// 311
	{ GLFW_KEY_F23, v3d::KeyCode::eF23 },						// 312
	{ GLFW_KEY_F24, v3d::KeyCode::eF24 },						// 313
	{ GLFW_KEY_F25, v3d::KeyCode::eF25 },						// 314
	{ GLFW_KEY_KP_0, v3d::KeyCode::eKP0 },						// 320
	{ GLFW_KEY_KP_1, v3d::KeyCode::eKP1 },						// 321
	{ GLFW_KEY_KP_2, v3d::KeyCode::eKP2 },						// 322
	{ GLFW_KEY_KP_3, v3d::KeyCode::eKP3 },						// 323
	{ GLFW_KEY_KP_4, v3d::KeyCode::eKP4 },						// 324
	{ GLFW_KEY_KP_5, v3d::KeyCode::eKP5 },						// 325
	{ GLFW_KEY_KP_6, v3d::KeyCode::eKP6 },						// 326
	{ GLFW_KEY_KP_7, v3d::KeyCode::eKP7 },						// 327
	{ GLFW_KEY_KP_8, v3d::KeyCode::eKP8 },						// 328
	{ GLFW_KEY_KP_9, v3d::KeyCode::eKP9 },						// 329
	{ GLFW_KEY_KP_DECIMAL, v3d::KeyCode::eKPDecimal },			// 330
	{ GLFW_KEY_KP_DIVIDE, v3d::KeyCode::eKPDivide },			// 331
	{ GLFW_KEY_KP_MULTIPLY, v3d::KeyCode::eKPMultiply },		// 332
	{ GLFW_KEY_KP_SUBTRACT, v3d::KeyCode::eKPSubtract },		// 333
	{ GLFW_KEY_KP_ADD, v3d::KeyCode::eKPAdd },					// 334
	{ GLFW_KEY_KP_ENTER, v3d::KeyCode::eKPEnter },				// 335
	{ GLFW_KEY_KP_EQUAL, v3d::KeyCode::eKPEqual },				// 336
	{ GLFW_KEY_LEFT_SHIFT, v3d::KeyCode::eLeftShift },			// 340
	{ GLFW_KEY_LEFT_CONTROL, v3d::KeyCode::eLeftControl },		// 341
	{ GLFW_KEY_LEFT_ALT, v3d::KeyCode::eLeftAlt },				// 342
	{ GLFW_KEY_LEFT_SUPER, v3d::KeyCode::eLeftSuper },			// 343
	{ GLFW_KEY_RIGHT_SHIFT, v3d::KeyCode::eRightShift },		// 344
	{ GLFW_KEY_RIGHT_CONTROL, v3d::KeyCode::eRightControl },	// 345
	{ GLFW_KEY_RIGHT_ALT, v3d::KeyCode::eRightAlt },			// 346
	{ GLFW_KEY_RIGHT_SUPER, v3d::KeyCode::eRightSuper },		// 347
	{ GLFW_KEY_MENU, v3d::KeyCode::eMenu },						// 348
};

InputManager::InputManager()
	: keyModifiers(v3d::KeyModifierBits::eNone)
	, blocked(false)
	, mouseScrollValueX(0)
	, mouseScrollValueY(0)
	, prevMousePos(0.0f)
	, curMousePos(0.0f)
	, keyBufferMode(false)
	, maxBufferSize(0)
	//, redirectingTextField(nullptr)
	//, gamecontrollerEnabled(false)
	//, onControllerConnected(nullptr)
	//, onControllerDisconnected(nullptr)
{}

InputManager::~InputManager()
{
	releaseAll();
}

v3d::InputManager* InputManager::get() { return v3d::Engine::get()->getInputManager(); }

bool InputManager::init()
{
	// Init mouse button vector
	mouseButtonInputs.resize(static_cast<int>(v3d::MouseButton::eEnumLast) - 1);

	// Init as released
	std::fill(std::begin(mouseButtonInputs), std::end(mouseButtonInputs), v3d::InputState::eReleased);

	// Clear mouse once map
	mouseButtonInputOnce.clear();

	// Init key vector
	keyInputs.resize(static_cast<int>(v3d::KeyCode::eEnumLast) - 1);
	
	// Init as released
	std::fill(std::begin(keyInputs), std::end(keyInputs), v3d::InputState::eReleased);

	// Clear key once map
	keyInputsOnce.clear();

	/*
	// flag
	bool useGamepad = false;

#if V3D_INPUT_GAMEPAD
	// using gamepad
	useGamepad = true;
#endif

	// check
	if (useGamepad)
	{
		// init sdl game controller and haptic (brrr)
		if (SDL_Init(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0)
		{
			// Failed to initialize SDL

			// Report
			v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::INPUT_MANAGER, v3d::Error::Code::InputErrorCode::FAILED_TO_INIT_SDL));

#if V3D_INPUT_SDL_FAIL_TERMINATE
			// SDL is required. 
			return false;
#endif
		}
		else
		{
			// Successfully initialize SDL
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
			v3d::Logger::getInstance().info("[System] Gamepads are supported.");
#endif

			// set flag
			gamecontrollerEnabled = true;

			// poll SDL event.
			pollGameControllerInput();
		}
	}
	else
	{
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
		v3d::Logger::getInstance().info("[System] Gamepads are not supported.");
#endif
	}
	*/

	// Done
	return true;
}

void InputManager::blockInput(const bool enabled)
{
	blocked = enabled;
}

glm::vec2 InputManager::getMousePosition() const
{
	return curMousePos;
}

glm::vec2 InputManager::getMouseMovedDistance() const
{
	return curMousePos - prevMousePos;
}

bool InputManager::didMouseMove() const
{
	return prevMousePos != curMousePos;
}

bool InputManager::isMouseButtonPressed(const v3d::MouseButton mouseButton, const bool currentFrame) const
{
	if (mouseButton != v3d::MouseButton::eEnumLast)
	{
		if (currentFrame)
		{
			// Check if mouse button was pressed on current frame
			auto find_mouse_it = mouseButtonInputOnce.find(mouseButton);

			if (find_mouse_it == mouseButtonInputOnce.end())
			{
				// Didn't release
				return false;
			}
			else
			{
				return find_mouse_it->second == v3d::InputState::ePressed;
			}
		}
		else
		{
			// Check if mouse button is pressed
			return mouseButtonInputs.at(static_cast<int>(mouseButton)) == v3d::InputState::ePressed;
		}
	}
	else
	{
		return false;
	}
}

bool InputManager::isMouseButtonReleased(const v3d::MouseButton mouseButton, const bool currentFrame) const
{
	if (mouseButton != v3d::MouseButton::eEnumLast)
	{
		if (currentFrame)
		{
			// Check if mouse button was rleased on current frame
			auto find_mouse_it = mouseButtonInputOnce.find(mouseButton);

			if (find_mouse_it == mouseButtonInputOnce.end())
			{
				// Didn't release
				return false;
			}
			else
			{
				return find_mouse_it->second == v3d::InputState::eReleased;
			}
		}
		else
		{
			// Check if mouse button is released
			return mouseButtonInputs.at(static_cast<int>(mouseButton)) == v3d::InputState::eReleased;
		}
	}
	else
	{
		return false;
	}
}

bool InputManager::isAnyMouseButtonPressed(const bool currentFrame) const
{
	if (currentFrame)
	{
		for (auto& e : mouseButtonInputOnce)
		{
			if ((e.second) == v3d::InputState::ePressed)
			{
				return true;
			}
		}

		return false;
	}
	else
	{
		// todo: This iterates all mouse buttons, which
		const std::size_t len = static_cast<std::size_t>(v3d::MouseButton::eEnumLast);
		for (std::size_t i = 0; i < len; ++i)
		{
			if (mouseButtonInputs.at(i) == v3d::InputState::ePressed)
			{
				return true;
			}
		}

		return false;
	}
}

bool InputManager::isAnyMouseButtonReleased(const bool currentFrame) const
{
	if (currentFrame)
	{
		for (auto& e : mouseButtonInputOnce)
		{
			if ((e.second) == v3d::InputState::eReleased)
			{
				return true;
			}
		}

		return false;
	}
	else
	{
		// todo: This iterates all mouse buttons, which
		const std::size_t len = static_cast<std::size_t>(v3d::MouseButton::eEnumLast);
		for (std::size_t i = 0; i < len; ++i)
		{
			if (mouseButtonInputs.at(i) == v3d::InputState::eReleased)
			{
				return true;
			}
		}

		return false;
	}
}

int InputManager::getMouseScrollXValue() const
{
	return mouseScrollValueX;
}

int InputManager::getMouseScrollYValue() const
{
	return mouseScrollValueY;
}

bool InputManager::isKeyPressed(const v3d::KeyCode keyCode, const bool currentFrame) const
{
	if (keyCode != v3d::KeyCode::eEnumLast)
	{
		if (currentFrame)
		{
			// Check if key was pressed on current frame
			auto find_key_it = keyInputsOnce.find(keyCode);

			if (find_key_it == keyInputsOnce.end())
			{
				// Didn't press
				return false;
			}
			else
			{
				// true if was pressed
				return find_key_it->second == v3d::InputState::ePressed;
			}
		}
		else
		{
			// Check if key is pressing
			return keyInputs.at(static_cast<int>(keyCode)) == v3d::InputState::ePressed;
		}
	}
	else
	{
		return false;
	}
}

bool InputManager::isKeyReleased(const v3d::KeyCode keyCode, const bool currentFrame) const
{
	if (keyCode != v3d::KeyCode::eEnumLast)
	{
		if (currentFrame)
		{
			// Check if key was released on current frame
			auto find_key_it = keyInputsOnce.find(keyCode);

			if (find_key_it == keyInputsOnce.end())
			{
				// Didn't release
				return false;
			}
			else
			{
				// true if was released
				return find_key_it->second == v3d::InputState::eReleased;
			}
		}
		else
		{
			// Check if key is pressing
			return keyInputs.at(static_cast<int>(keyCode)) == v3d::InputState::eReleased;
		}
	}
	else
	{
		return false;
	}
}

bool InputManager::isKeyModifierPressing(const v3d::KeyModifierBits keyModifier) const
{
	return (keyModifiers & keyModifier);
}

bool InputManager::isKeyModifierPressing(const unsigned int keyModifier) const
{
	return (keyModifiers & keyModifier);
}

v3d::KeyModifierBits InputManager::getKeyModifier() const
{
	return keyModifiers;
}

/*
void InputManager::setCursorToCenter()
{
	glm::ivec2 res = v3d::Engine::getInstance().getGLView().getWindowSize();

	prevMousePos = curMousePos = (glm::vec2(res) * 0.5f);
}
*/

void InputManager::updateMousePosition(const int x, const int y)
{
	if (blocked) return;

	curMousePos = glm::vec2(x, y);
}

void InputManager::updateMouseButtonInput(const int button, const int action, const int mods)
{
	if (blocked) return;

	if (0 <= button && button <= static_cast<int>(v3d::MouseButton::eEnumLast))
	{
		// valid button
		if (action == GLFW_PRESS)
		{
			// mouse button pressed
			mouseButtonInputs.at(button) = v3d::InputState::ePressed;
			mouseButtonInputOnce[static_cast<v3d::MouseButton>(button)] = v3d::InputState::ePressed;
		}
		else if (action == GLFW_RELEASE)
		{
			// mouse button released
			mouseButtonInputs.at(button) = v3d::InputState::eReleased;
			mouseButtonInputOnce[static_cast<v3d::MouseButton>(button)] = v3d::InputState::eReleased;
		}
	}

	// Update mod
	keyModifiers = static_cast<v3d::KeyModifierBits>(mods);
}

void InputManager::updateMouseScrollValue(const double xOffset, const double yOffset)
{
	if (blocked) return;

	mouseScrollValueX = static_cast<int>(xOffset);
	mouseScrollValueY = static_cast<int>(yOffset);
}

void InputManager::updateKeyInput(const int glfwKey, const int action, const int mods)
{
	if (blocked) return;

	// find key code by GLFW key
	auto find_keycode = glfwToKeyCodeMap.find(glfwKey);

	if (find_keycode != glfwToKeyCodeMap.end())
	{
		// key code found
		const v3d::KeyCode keyCode = find_keycode->second;

		/*
		// check if has redirecting node
		if (redirectingTextField && action != GLFW_RELEASE)
		{
			// redirect key input
			redirectingTextField->appendKey(keyCode, static_cast<v3d::KeyModifier>(mods));

			// skip update
			return;
		}

		*/
		// get index
		const int index = static_cast<int>(keyCode);
		const int maxIndex = (int)keyInputs.size();

		if (0 <= index && index < maxIndex)
		{
			// valid index
			if (action == GLFW_PRESS)
			{
				// Key pressed
				keyInputs.at(index) = v3d::InputState::ePressed;
				keyInputsOnce[keyCode] = v3d::InputState::ePressed;
			}
			else if (action == GLFW_RELEASE)
			{
				// Key released
				keyInputs.at(index) = v3d::InputState::eReleased;
				keyInputsOnce[keyCode] = v3d::InputState::eReleased;
			}

			if (keyBufferMode)
			{
				if (keyBuffer.size() >= maxBufferSize)
				{
					keyBuffer.pop_front();
				}

				keyBuffer.push_back(v3d::keyCodeToChar(keyCode, mods & v3d::KeyModifierBits::eShift));
			}
		}
		// Else, invalid index
	}

	// Update mod
	keyModifiers = static_cast<v3d::KeyModifierBits>(mods);
}

void InputManager::postUpdate()
{
	keyInputsOnce.clear();

	mouseButtonInputOnce.clear();

	mouseScrollValueX = 0;
	mouseScrollValueY = 0;

	//for (auto& gc : gameControllers)
	//{
	//	(gc.second)->postUpdate();
	//}

	prevMousePos = curMousePos;
}

/*
void InputManager::pollGameControllerInput()
{
	if (!gamecontrollerEnabled) return;

	SDL_Event e;
	// Poll every event
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_CONTROLLERDEVICEADDED:
		{
			addController(e.cdevice);
		}
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
		{
			removeController(e.cdevice);
		}
			break;
		case SDL_CONTROLLERBUTTONDOWN:
		{
			controllerButtonPressed(e.cdevice.which, e.cbutton);
		}
			break;
		case SDL_CONTROLLERBUTTONUP:
		{
			controllerButtonReleased(e.cdevice.which, e.cbutton);
		}
			break;
		case SDL_CONTROLLERAXISMOTION:
		{
			controllerAxisMoved(e.cdevice.which, e.caxis);
		}
			break;
		default:
			break;
		}
	}
}

void InputManager::addController(const SDL_ControllerDeviceEvent event)
{
	if (!gamecontrollerEnabled) return;

	if (SDL_IsGameController(event.which))
	{
		// Game controller event
		SDL_GameController* newController = SDL_GameControllerOpen(event.which);

		// Check
		if (newController == nullptr)
		{
			// Failed to get new controller
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
			v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::INPUT_MANAGER, v3d::Error::Code::InputErrorCode::FAILED_TO_GET_SDL_GAMECONTROLLER));
#endif
			// Failed.
			return;
		}
		else
		{
			// Conver to joystick
			SDL_Joystick* joystick = SDL_GameControllerGetJoystick(newController);

			// Check
			if (joystick == nullptr)
			{
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
				v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::INPUT_MANAGER, v3d::Error::Code::InputErrorCode::FAILED_TO_GET_SDL_JOYSTICK));
#endif
				// Failed.
				return;
			}
			else
			{
				// Get instance id (game controller id)
				const int16_t instanceID = SDL_JoystickInstanceID(joystick);

				// Check
				if(instanceID == -1)
				{
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
					v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::INPUT_MANAGER, v3d::Error::Code::InputErrorCode::FAILED_TO_GET_JOYSTICK_ID));
#endif
					// Failed.
					return;
				}
				else
				{
					// Check duplication
					auto find_it = gameControllers.find(instanceID);
					if (find_it == gameControllers.end())
					{
						// Get name of controller
						std::string name = std::string(SDL_JoystickName(joystick));

						// Check if name is empty.
						if (name.empty())
						{
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
							// Info if name is empty
							v3d::Logger::getInstance().info("New game controller does not have a name.");
#endif

							// Set name to unknown with id
							name = "Unknown #" + std::to_string(instanceID);
						}

						// Get number of buttons and axis on controller
						const int buttonCount = SDL_JoystickNumButtons(joystick);
						const int axisCount = SDL_JoystickNumAxes(joystick);

						// Check count
						if ((buttonCount + axisCount) == 0)
						{
							// There is no button or axis on this controller
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
							v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::INPUT_MANAGER, v3d::Error::Code::InputErrorCode::GAME_PAD_HAS_NO_BUTTONS));
#endif

							// release sdl stuff
							SDL_JoystickClose(joystick);
							SDL_GameControllerClose(newController);

							// Failed.
							return;
						}

						// haptic instance
						SDL_Haptic* newHaptic = nullptr;

						// flag
						bool hapticEnabled = false;

						// Check
						if (SDL_JoystickIsHaptic(joystick) == 1)
						{
							// This controller has haptic
							newHaptic = SDL_HapticOpenFromJoystick(joystick);

							if (newHaptic == nullptr)
							{
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
								// Info if gamepad has haptic but failed to use.
								v3d::Logger::getInstance().warn("[System] Game controller \"" + name + "\" (ID: " + std::to_string(instanceID) + ") has haptic but failed to use.");
#endif
							}
							else
							{
								// Check if can rumble
								if (SDL_HapticRumbleSupported(newHaptic))
								{
									// Initialize haptic
									if (SDL_HapticRumbleInit(newHaptic))
									{
										// Failed
										hapticEnabled = false;
									}
									else
									{
										// Success.
										hapticEnabled = true;
									}
								}
								// Else, rumble not supported.
							}
						}
						else
						{
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
							// Info if gamepad has haptic but failed to use.
							v3d::Logger::getInstance().info("[System] Game controller \"" + name + "\" (ID: " + std::to_string(instanceID) + ") has haptic but failed to use.");
#endif
						}

						// SDL all ready.

						// Create instance
						v3d::GameController* newGameController = new (std::nothrow) v3d::GameController();

						// Check
						if (newGameController == nullptr)
						{
							// Report
							v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::GAME_CONTROLLER);

							// Check haptic and close if exist.
							if (newHaptic) SDL_HapticClose(newHaptic);

							// Close SDL stuff.
							SDL_JoystickClose(joystick);
							SDL_GameControllerClose(newController);

							// Failed.
							return;
						}

						// Init
						newGameController->controller = newController;
						newGameController->haptic = newHaptic;
						newGameController->hapticEnabled = hapticEnabled;
						newGameController->name = name;
						newGameController->id = instanceID;

						// Store new game controller
						gameControllers[instanceID] = std::move(std::shared_ptr<v3d::GameController>(newGameController));

#if V3D_LOG_INFO
						v3d::Logger::getInstance().info("Game controller is connected: " + name);
#endif

#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
						newGameController->print();
#endif

						// User callback
						if (onControllerConnected)
						{
							onControllerConnected(instanceID);
						}
					}
					else
					{
						// Same instance id exists

#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
						v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::INPUT_MANAGER, v3d::Error::Code::InputErrorCode::SAME_GAME_PAD_ID_EXISTS), "ID: " + std::to_string(instanceID));
#endif

						// Failed
						return;
					}
				}
			}
		}
	}
}

void InputManager::removeController(const SDL_ControllerDeviceEvent event)
{
	if (!gamecontrollerEnabled) return;

	auto find_it = gameControllers.find(event.which);

	if (find_it == gameControllers.end())
	{
		// There isn't controller to remove
		return;
	}
	else
	{
		// Detach
		(find_it->second)->detached = true;

#if V3D_LOG_INFO
		v3d::Logger::getInstance().info("Game controller is disconnected: " + (find_it->second)->getName());
#endif

#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
		(find_it->second)->print();
#endif

		// Deref shared ptr
		gameControllers.erase(event.which);

		// User callback
		if (onControllerDisconnected)
		{
			onControllerDisconnected(event.which);
		}
	}
}

void InputManager::controllerButtonPressed(const int16_t id, const SDL_ControllerButtonEvent event)
{
	if (blocked) return;

	if (!gamecontrollerEnabled) return;

	// Check id
	if (id == -1) return;

	// Check state
	if (event.state == SDL_PRESSED)
	{
		// button pressed

		// Get button
		v3d::Gamepad::Button button = static_cast<v3d::Gamepad::Button>(event.button);

		// Find controller
		std::shared_ptr<v3d::GameController> gc = getGameController(id);

		if (button == v3d::Gamepad::Button::BUTTON_ENUM_LAST)
		{
			// wrong enum
			return;
		}
		else
		{
			// Find controller
			std::shared_ptr<v3d::GameController> gc = getGameController(id);

			if (gc)
			{
				gc->updateButtonState(button, v3d::InputState::ePressed);
			}
			else
			{
				// There is no controller
				return;
			}
		}
	}
}

void InputManager::controllerButtonReleased(const int16_t id, const SDL_ControllerButtonEvent event)
{
	if (blocked) return;

	if (!gamecontrollerEnabled) return;

	// Check id
	if (id == -1) return;

	// Check state
	if (event.state == SDL_RELEASED)
	{
		// button released

		// Get button
		v3d::Gamepad::Button button = static_cast<v3d::Gamepad::Button>(event.button);

		if (button == v3d::Gamepad::Button::BUTTON_ENUM_LAST)
		{
			// wrong enum
			return;
		}
		else
		{
			// Find controller
			std::shared_ptr<v3d::GameController> gc = getGameController(id);

			if (gc)
			{
				gc->updateButtonState(button, v3d::InputState::eReleased);
			}
			else
			{
				// There is no controller
				return;
			}
		}
	}
}

void InputManager::controllerAxisMoved(const int16_t id, const SDL_ControllerAxisEvent event)
{
	if (blocked) return;
	
	if (!gamecontrollerEnabled) return;

	// Check id
	if (id == -1) return;

	// Find controller
	std::shared_ptr<v3d::GameController> gc = getGameController(id);

	// Get axis
	v3d::Gamepad::Axis axis = static_cast<v3d::Gamepad::Axis>(event.axis);

	if (axis == v3d::Gamepad::Axis::AXIS_ENUM_LAST)
	{
		// wrong enum
		return;
	}
	else
	{
		if (gc)
		{
			gc->updateAxisValue(axis, event.value);
		}
		else
		{
			// There is no controller
			return;
		}
	}
}

std::vector<std::shared_ptr<v3d::GameController>> InputManager::getAllGameControllers()
{
	std::vector<std::shared_ptr<v3d::GameController>> gcs;

	for (auto& gc : gameControllers)
	{
		gcs.push_back(gc.second);
	}

	return gcs;
}

std::shared_ptr<v3d::GameController> InputManager::getGameController(const int16_t id)
{
	if (id == -1) return nullptr;

	if (gameControllers.empty())
	{
		return nullptr;
	}
	else
	{
		auto find_it = gameControllers.find(id);

		if (find_it == gameControllers.end())
		{
			return nullptr;
		}
		else
		{
			return (find_it->second);
		}
	}
}
*/

void InputManager::enableKeyBufferMode(const std::size_t maxBufferSize)
{
	if (!keyBufferMode)
	{
		keyBufferMode = true;

		keyBuffer.clear();
		this->maxBufferSize = maxBufferSize;
	}
}

void InputManager::disableKeyBufferMode()
{
	if (keyBufferMode)
	{
		keyBufferMode = false;
		maxBufferSize = 0;
		keyBuffer.clear();
	}
}

char InputManager::getCharFromBuffer()
{
	if (keyBufferMode)
	{
		if (!keyBuffer.empty())
		{
			char c = keyBuffer.front();
			keyBuffer.pop_front();
			return c;
		}
	}

	return 0;
}

std::size_t InputManager::getKeyBufferSize() const
{
	return keyBuffer.size();
}

std::string InputManager::getBufferAsString(const bool release)
{
	if (keyBufferMode)
	{
		if (!keyBuffer.empty())
		{
			auto str = std::string(keyBuffer.begin(), keyBuffer.end());

			if (release)
			{
				keyBuffer.clear();
			}

			return str;
		}
	}

	return "";
}

bool InputManager::isKeyBufferModeEnabled() const
{
	return keyBufferMode;
}

/*
bool InputManager::isGameControllerButtonPressed(const int16_t gameControllerID, const v3d::Gamepad::Button button, const bool currentFrame)
{
	// get controller
	std::shared_ptr<v3d::GameController> gc = getGameController(gameControllerID);

	if (gc)
	{
		// Found game controller
		return gc->isButtonPressed(button, currentFrame);
	}
	else
	{
		// Can't find game controller with id
		return false;
	}
}

bool InputManager::isGameControllerButtonReleased(const int16_t gameControllerID, const v3d::Gamepad::Button button, const bool currentFrame)
{
	// get controller
	std::shared_ptr<v3d::GameController> gc = getGameController(gameControllerID);

	if (gc)
	{
		// Found game controller
		return gc->isButtonReleased(button, currentFrame);
	}
	else
	{
		// Can't find game controller with id
		return false;
	}
}

int16_t InputManager::getGameControllerAxisValue(const int16_t gameControllerID, const v3d::Gamepad::Axis axis)
{
	// get controller
	std::shared_ptr<v3d::GameController> gc = getGameController(gameControllerID);

	if (gc)
	{
		// Found game controller
		return gc->getAxisValue(axis);
	}
	else
	{
		// Can't find game controller with id
		return 0;
	}
}
*/

void InputManager::resetAllKeyStates()
{
	std::fill(std::begin(keyInputs), std::end(keyInputs), v3d::InputState::eReleased);
	keyInputsOnce.clear();
}

void InputManager::resetAllMosueButtonStates()
{
	std::fill(std::begin(mouseButtonInputs), std::end(mouseButtonInputs), v3d::InputState::eReleased);
	mouseButtonInputOnce.clear();
}

void InputManager::resetInputStates()
{
	resetAllMosueButtonStates();

	mouseScrollValueX = 0;
	mouseScrollValueY = 0;

	resetAllKeyStates();

	keyModifiers = v3d::KeyModifierBits::eNone;

	prevMousePos = glm::vec2(0.0f);
	curMousePos = glm::vec2(0.0f);

	//for (auto& gc : gameControllers)
	//{
	//	(gc.second)->resetInputStates();
	//}
}

void InputManager::releaseAll()
{
	resetInputStates();

	//if (gamecontrollerEnabled)
	//{
	//	gameControllers.clear();

	//	// Close SDL
	//	SDL_Quit();
	//	gamecontrollerEnabled = false;
	//}
}

V3D_NS_END