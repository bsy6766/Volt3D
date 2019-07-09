/**
*	@file Application.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_APPLICATION_H
#define V3D_APPLICATION_H

#include <string>
#include <string_view>

#include "Utils/Macros.h"

namespace v3d
{
	class Engine;

	class VOLT3D_DLL Application
	{
	protected:
		v3d::Application* sharedInstance;
		std::wstring name;
		//v3d::Engine* engine;

	public:
		Application();
		Application(const wchar_t* appName);
		~Application();

		//static v3d::Application& getInstance()
		//{
		//	static v3d::Application instance;
		//	return instance;
		//}
		
		void start(const char* windowTitle);
		void start(v3d::Engine& engine, const char* windowTitle);

		std::wstring getName() const { return name; }
		//v3d::Engine* getEngine() const { return engine; }
	};
}

#endif