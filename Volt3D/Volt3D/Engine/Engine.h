/**
*	@file Engine.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_ENGINE_H
#define V3D_ENGINE_H

#include <string_view>

#include "Utils/Macros.h"

V3D_NS_BEGIN

class Application;
class Time;
class Director;
class Preference;
class InputManager;
class TextureCache;
namespace glfw { class Window; }
namespace vulkan { class Context; }

/**
*	@class Engine
*
*	@group Engine
*
*	@since 1.0
*/
class VOLT3D_DLL Engine
{
	friend class v3d::Application;

private:
	Engine();

	static v3d::Engine* instance;

	v3d::glfw::Window* window;
	v3d::Time* time;

	v3d::vulkan::Context* context;

	v3d::Director* director;
	v3d::Preference* preference;

	std::unique_ptr<v3d::InputManager> inputManager;
	std::unique_ptr<v3d::TextureCache> textureCache;

	bool loadPreference(const std::wstring& folderName);
	bool initWindow(const char* windowTitle);
	bool initContext();
	void release();

	void preUpdate();
	void update(const float delta);
	void postUpdate(const float delta);
	void render();

	/**
	*	Initialize the engine.
	*	@param windowTitle The title of the window
	*	@param folderName The name of application folder name to be created (if doesn't exist)
	*	@return true if successfully initializes. Else, false.
	*/
	bool init(const char* windowTitle, const std::wstring& folderName);
		
	/** Run the engine */
	void run();

public:
	~Engine();

	/** Get Engine instance */
	static v3d::Engine* get() { return v3d::Engine::instance; }

	/** End the engine. Closes the window. */
	void end();

	/** Get window */
	v3d::glfw::Window* getWindow() const;

	/** Get time */
	v3d::Time* getTime() const;

	/** Get Vulkan context */
	v3d::vulkan::Context* getVulkanContext() const;

	/** Get Director */
	v3d::Director* getDirector() const;

	/** Get InputManager */
	v3d::InputManager& getInputManager() const;

	/** Get TextureCache */
	v3d::TextureCache& getTextureCache() const;
};

V3D_NS_END

#endif