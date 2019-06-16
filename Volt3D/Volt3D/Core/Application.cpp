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

v3d::Application::Application(std::string_view name)
	: name("")
{}

v3d::Application::~Application() {}

void v3d::Application::start()
{
	v3d::Engine engine = v3d::Engine();
	start(engine);
	std::cout << "Start!\n";
}

void v3d::Application::start(v3d::Engine& engine)
{
	try
	{
		if (engine.init(name))
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

std::string v3d::Application::getName() const
{
	return name;
}
