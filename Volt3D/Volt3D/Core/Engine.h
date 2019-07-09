﻿/**
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
	class InputManager;
	class Director;
	class Preference;
	namespace glfw { class Window; class Time; }
	namespace vulkan { class Context; }

	/**
	*	@class Engine
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Engine
	{
	protected:
		v3d::glfw::Window* window;
		v3d::glfw::Time* time;
		v3d::vulkan::Context* context;
		v3d::InputManager* inputManager;
		v3d::Director* director;
		v3d::Preference* preference;

		bool loadPreference(const std::wstring& folderName);
		bool initWindow(const char* windowTitle);
		bool initContext();
		void release();

		void preUpdate(const float delta);
		void update(const float delta);
		void postUpdate(const float delta);
		void render();
		
	public:
		Engine();
		~Engine();

		bool init(const char* windowTitle, const std::wstring& folderName);
		void run();
		void end();

		v3d::glfw::Window& getView() const;
		v3d::vulkan::Context& getVulkanContext() const;
	};
}

#endif