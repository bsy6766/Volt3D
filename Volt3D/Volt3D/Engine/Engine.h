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

namespace v3d
{
	class Time;
	class Director;
	class Preference;
	class InputManager;
	class TextureManager;
	namespace glfw { class Window; }
	namespace vulkan { class Context; }

	/**
	*	@class Engine
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Engine
	{
	private:
		static v3d::Engine* instance;

	protected:
		v3d::glfw::Window* window;
		v3d::Time* time;

		v3d::vulkan::Context* context;

		v3d::Director* director;
		v3d::Preference* preference;

		v3d::InputManager* inputManager;
		std::shared_ptr<v3d::TextureManager> textureManager;

		bool loadPreference(const std::wstring& folderName);
		bool initWindow(const char* windowTitle);
		bool initContext();
		bool initTextureManager();
		void release();

		void preUpdate(const float delta);
		void update(const float delta);
		void postUpdate(const float delta);
		void render();
		
	public:
		Engine();
		~Engine();

		static v3d::Engine* get() { return v3d::Engine::instance; }

		bool init(const char* windowTitle, const std::wstring& folderName);
		void run();
		void end();

		v3d::glfw::Window* getWindow() const;
		v3d::Time* getTime() const;
		v3d::vulkan::Context* getVulkanContext() const;
		v3d::Director* getDirector() const;
		v3d::InputManager* getInputManager() const;
	};
}

#endif