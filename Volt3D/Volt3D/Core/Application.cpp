/**
*	@file Application.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Application.h"

#include <iostream>

#include "Engine.h"
#include "Utils/Logger.h"

v3d::Application::Application()
	: name(L"Volt3D App")
	, sharedInstance(this)
	//, engine(new v3d::Engine())
{}

v3d::Application::Application(const wchar_t* appName)
	: name(appName)
	, sharedInstance(this)
	//, engine(new v3d::Engine())
{}

v3d::Application::~Application() 
{
	//SAFE_DELETE(engine);
}

void v3d::Application::start(const char* windowTitle)
{
	v3d::Engine engine = v3d::Engine();
	start(engine, windowTitle);
}

void v3d::Application::start(v3d::Engine& engine, const char* windowTitle)
{
	try
	{
		if (engine.init(windowTitle, name))
		{
			engine.run();
		}
	}
	catch (vk::SystemError err)
	{
		v3d::Logger::getInstance().critical("vk::SystemError: " + std::string(err.what()));
		exit(-1);
	}
	catch (std::runtime_error err)
	{
		v3d::Logger::getInstance().critical("std::runtime_error: " + std::string(err.what()));
		exit(-1);
	}
	catch (...)
	{
		v3d::Logger::getInstance().critical("unknown error");
		exit(-1);
	}
}
