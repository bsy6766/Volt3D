/**
*	@file InputManager.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_INPUT_MANAGER_H
#define V3D_INPUT_MANAGER_H

#include <string>
#include <unordered_map>
#include <memory>		// std::unique_ptr
#include <mutex>
#include <list>
#include <functional>

#include <glm/glm.hpp>
//#include <SDL2/SDL.h>

#include "KeyCode.h"
#include "KeyModifierBits.h"
#include "MouseButton.h"
#include "InputState.h"

#include "Utils/Macros.h"

//#include "Volt3D/UI/TextField.h"

V3D_NS_BEGIN

class Engine;
class GameController;
namespace glfw { class Window; }

/**
*	@class InputManager
*	@brief Manges keyboard, mouse, and gamepad inputs.
*	
*	@note Uses GLFW for keyboard and mouse.
*	@todo Reimplement gamepads with XInput
*
*	All inputs are updated first than any other systems in the application.
*
*	All inputs are polled before any update every frame in main thread.
*/
class VOLT3D_DLL InputManager
{
	friend class v3d::Engine;
	friend class v3d::glfw::Window;
	//friend class Redirector;

private:
	// Private constructor. To get InputManager instance, use GLView's getInputManager().
	InputManager();

	/** flag. all inputs are blocked if this is true. */
	bool blocked;

	bool init();
	void releaseAll();
	void resetInputStates();

	glm::vec2 prevMousePos;
	glm::vec2 curMousePos;

	// -1 = left, down / 0 = idle / 1 = right, up
	int mouseScrollValueX;
	int mouseScrollValueY;


	// Mouse button inputs. Index equals to v3d::MouseButton
	std::vector<v3d::InputState> mouseButtonInputs;
	// Mouse button input for current frame
	std::unordered_map<v3d::MouseButton, v3d::InputState> mouseButtonInputOnce;


	// GLFW key to KeyCode map
	static std::unordered_map<int/*GLFW define*/, v3d::KeyCode> glfwToKeyCodeMap;
	// Keyboard inputs. Index equals to v3d::KeyCode.
	std::vector<v3d::InputState> keyInputs;
	// Keyboard input for current frame. 
	std::unordered_map<v3d::KeyCode, v3d::InputState> keyInputsOnce;
	// Key modifier (ctrl, shift, etc). Used for both keyboard and mouse
	v3d::KeyModifierBits keyModifiers;


	// If buffer mode is enabled, all keyboard input will be stored in buffer
	bool keyBufferMode;
	// max buffer size
	std::size_t maxBufferSize;
	// buffer.
	std::list<char> keyBuffer;


	// ================== Redirection ===================
	// InputManager redirects all keyboard input to this node.
	//v3d::UI::TextField* redirectingTextField;
	// ==================================================


	// ================== Update Input ==================
	// Update mouse position
	void updateMousePosition(const int x, const int y);
	// Update mouse button
	void updateMouseButtonInput(const int button, const int action, const int mods);
	// Update mouse scroll
	void updateMouseScrollValue(const double xOffset, const double yOffset);
	// Update key input
	void updateKeyInput(const int glfwKey, const int action, const int mods);
	// Post update. Resets once inputs
	void postUpdate();
	// ==================================================


	// ===================== Gamepad ====================
	//bool gamecontrollerEnabled;
	//// Game controllers
	//std::unordered_map<int16_t/*Game controller id*/, std::shared_ptr<v3d::GameController>> gameControllers;
	//// Called when game controller is connected
	//void addController(const SDL_ControllerDeviceEvent event);
	//// Called when game controller is removed
	//void removeController(const SDL_ControllerDeviceEvent event);
	//// Called when game controller's button is pressed
	//void controllerButtonPressed(const int16_t id, const SDL_ControllerButtonEvent event);
	//// Called when game controller's button is released
	//void controllerButtonReleased(const int16_t id, const SDL_ControllerButtonEvent event);
	//// Called when game controller's axis is moved
	//void controllerAxisMoved(const int16_t id, const SDL_ControllerAxisEvent event);
	//// poll gamecontroller input
	//void pollGameControllerInput();
	// ==================================================

public:
	// Destructor. Releases all shaders
	~InputManager();

	/** Get InputManager instance */
	static v3d::InputManager* get();

	/**
	*	Block input.
	*	In enabled, all inputs from keyboard, mouse, and gamepad will be blocked.
	*	@param enabled true to enable input block. Else, false.
	*/
	void blockInput(const bool enabled);
				
	/**
	*	Get current mouse position
	*	Mouse position starts from top-left corner.
	*	@return Current mouse position
	*/
	glm::vec2 getMousePosition() const;

	/**
	*	Get moved distance of mouse on current frame.
	*	@return Distance between previous mouse position and current position.
	*/
	glm::vec2 getMouseMovedDistance() const;

	/** Check if mouse moved on current frame */
	bool didMouseMove() const;

	/**
	*	Check if mouse button was pressed
	*	@param mouseButton A mouse button to check
	*	@param currentFrame To check if mouse button was pressed on current frame, set true. To check if mouse button is pressing, set false.
	*	@return true if mouse button was pressed. Else, false.
	*/
	bool isMouseButtonPressed(const v3d::MouseButton mouseButton, const bool currentFrame) const;

	/**
	*	Check if mouse button was released
	*	@param mouseButton A mouse button to check
	*	@param currentFrame To check if mouse button was released on current frame, set true. To check if mouse button is releasing, set false.
	*	@return true if mouse button was released. Else, false.
	*/
	bool isMouseButtonReleased(const v3d::MouseButton mouseButton, const bool currentFrame) const;

	/**
	*	Check if any mouse button was pressed
	*	@param currentFrame Check if any mouse button was pressed on current frame.
	*	@return true if any mouse button was pressed.
	*/
	bool isAnyMouseButtonPressed(const bool currentFrame) const;

	/**
	*	Check if any mouse button was pressed
	*	@param currentFrame Check if any mouse button was pressed on current frame.
	*	@return true if any mouse button was pressed.
	*/
	bool isAnyMouseButtonReleased(const bool currentFrame) const;

	/**
	*	Get mouse scroll X value
	*	@return 0 if scroll is idle. -1 if scrolled left. 1 if scrolled right.
	*/
	int getMouseScrollXValue() const;

	/**
	*	Get mouse scroll Y value
	*	@return 0 if scroll is idle. -1 if scrolled down. 1 if scrolled up.
	*/
	int getMouseScrollYValue() const;

	/**
	*	Check if key was pressed.
	*	@param keyCode A key to check.
	*	@param currentFrame To check if key was pressed on current frame, set true. To check if key is pressing, set false.
	*	@return true if key was pressed. Else, false.
	*/
	bool isKeyPressed(const v3d::KeyCode keyCode, const bool currentFrame) const;

	/**
	*	Check if key was released.
	*	@param keyCode A key to check.
	*	@param currentFrame To check if key was released on current frame, set true. To check if key is releasing, set false.
	*	@return true if key was released. Else, false.
	*/
	bool isKeyReleased(const v3d::KeyCode keyCode, const bool currentFrame) const;

	/**
	*	Check if key modifier (ctrl, alt, shift, etc)
	*	@param keyModifier A enum bitwise of key modifier.
	*	@return true if keyModifier is pressing. Else, false.
	*/
	bool isKeyModifierPressing(const v3d::KeyModifierBits keyModifier) const;

	/**
	*	Check if key modifier (ctrl, alt, shift, etc)
	*	@param keyModifier A enum bitwise of key modifier.
	*	@return true if keyModifier is pressing. Else, false.
	*/
	bool isKeyModifierPressing(const unsigned int keyModifier) const;

	/** Get current key modifer */
	v3d::KeyModifierBits getKeyModifier() const;

	/**
	*	Enable key buffer mode.
	*	All keyboard input will be stored in buffer.
	*	If buffer is full, first character in buffer will be removed and new char will be pushed back.
	*/
	void enableKeyBufferMode(const std::size_t maxBufferSize = 1024);

	/**
	*	Disable key buffer mode.
	*	This will release entire buffer. 
	*/
	void disableKeyBufferMode();

	/**
	*	Get first char from buffer
	*	@return First character in buffer. Returns 0 if key buffer mode is not enabled or buffer is empty
	*/
	char getCharFromBuffer();

	/**
	*	Get key buffer size
	*	@return Size of current key buffer size.
	*/
	std::size_t getKeyBufferSize() const;

	/**
	*	Get entire buffer in string
	*	@param release Buffer will be released if set to true. Else, false.
	*	@return An entire buffer in std::string. Returns empty string if key buffer mode is not enabled or buffer is empty.
	*/
	std::string getBufferAsString(const bool release);

	/**
	*	Check if key buffer mode is enabled
	*	@return true if enabled. Else, false.
	*/
	bool isKeyBufferModeEnabled() const;

	// Callback function when controller is connected
	//std::function<void(const int16_t id)> onControllerConnected;

	// Callback function when controller is disconnected
	//std::function<void(const int16_t id)> onControllerDisconnected;

	// Get all game controllers
	//std::vector<std::shared_ptr<v3d::GameController>> getAllGameControllers();

	// Get game controller by id
	//std::shared_ptr<v3d::GameController> getGameController(const int16_t id);

	/**
	*	Check if game controller button is pressed.
	*	@param gameControllerID A game controller id to check.
	*	@param button A button to check.
	*	@param currentFrame To check if button was pressed on current frame, set true. To check if button is pressing, set false.
	*	@return true if button is pressed. Else, false
	*/
	//bool isGameControllerButtonPressed(const int16_t gameControllerID, const v3d::Gamepad::Button button, const bool currentFrame);

	/**
	*	Check if button is released.
	*	@param gameControllerID A game controller id to check.
	*	@param button A button to check.
	*	@param currentFrame To check if button was released on current frame, set true. To check if button is releasing, set false.
	*	@return true if button is released. Else, false
	*/
	//bool isGameControllerButtonReleased(const int16_t gameControllerID, const v3d::Gamepad::Button button, const bool currentFrame);

	/**
	*	Get game controller axis value
	*	Up and Left have negative values. Down and Right have postiive value.
	*	@param gameControllerID A game controller id to check.
	*	@param axis An axis to get value.
	*	@return Get axis value in range of [-32768, 32767].
	*/
	//int16_t getGameControllerAxisValue(const int16_t gameControllerID, const v3d::Gamepad::Axis axis);

	// Reset all key states to released (even though physically pressing keyboard)
	void resetAllKeyStates();

	// Reset all mouse button states to released (even though physically pressing mouse button)
	void resetAllMosueButtonStates();

	// Set cursor to center of screen
	//void setCursorToCenter();
};

/*
class Redirector
{
	friend class v3d::UI::TextField;
	friend class Director;

private:
	static void setRedirectingNode(v3d::UI::TextField* redirectingTextField)
	{
		//if (v3d::InputManager::getInstance().redirectingTextField != nullptr)
		//{
		//	v3d::InputManager::getInstance().redirectingTextField->endEdit();
		//}

		InputManager::getInstance().redirectingTextField = redirectingTextField;
	}
};
*/


V3D_NS_END

#endif