﻿/**
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

using json = nlohmann::json;

V3D_NS_BEGIN

Application::Application()
	: name()
	, engine(nullptr)
{}

Application::~Application() {}

bool Application::init( const wchar_t* appName, const char* windowTitle )
{
	if (engine) return false;
	engine = new v3d::Engine();
	engine->init( windowTitle, std::wstring( appName ) );
	name = appName;
	return true;
}

void Application::run()
{
	try
	{
		engine->run();
	}
	catch (vk::SystemError err)
	{
		v3d::Logger::getInstance().critical( "vk::SystemError: " + std::string( err.what() ) );
		exit( -1 );
	}
	catch (std::runtime_error err)
	{
		v3d::Logger::getInstance().critical( "std::runtime_error: " + std::string( err.what() ) );
		exit( -1 );
	}
	catch (...)
	{
		v3d::Logger::getInstance().critical( "unknown error" );
		exit( -1 );
	}

	SAFE_DELETE( engine );
}

V3D_NS_END