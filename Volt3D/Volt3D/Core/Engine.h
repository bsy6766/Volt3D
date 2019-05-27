﻿/**
*	@file Engine.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_ENGINE_H
#define V3D_ENGINE_H

#include "Utils/Macros.h"

namespace v3d
{
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
		v3d::glfw::Window* window;
		v3d::vulkan::Context* context;

		bool loadPreference();
		bool initWindow(const std::string_view windowTitle);
		bool initContext();
		void release();

	public:
		Engine();
		~Engine();

		bool init(const std::string_view windowTitle);
		void run();

		v3d::glfw::Window& getView() const;
		//v3d::vulkan::Context& getVulkanContext() const;
	};
}

#endif