﻿#include <PreCompiled.h>

#include "Engine.h"

#include <WinBase.h>	// GetUserName
#include <Lmcons.h>		// UNLEN
#include <ShlObj.h>		// SHGetFolderPath

#include "utils/FileSystem.h"
#include "utils/Logger.h"

#include "Window.h"
//#include "Vulkan/Context.h"

v3d::Engine::Engine()
	: window(nullptr)
	, context(nullptr)
{
	v3d::Logger::getInstance().init(FileSystem::getWorkingDirectoryW(), L"log.txt");
	v3d::Logger::getInstance().initConsole();
}

v3d::Engine::~Engine()
{
	release();
}

bool v3d::Engine::init(const std::string_view windowTitle)
{
	auto& logger = Logger::getInstance();

	if (!loadPreference()) return false;
	if (!initWindow(windowTitle)) return false;


	return true;
}

void v3d::Engine::run()
{
	while (window && window->isRunning())
	{
		window->pollGLFWEvent();
	}
}

v3d::glfw::Window& v3d::Engine::getView() const
{
	// @todo: Handle error
	return *window;
}

//v3d::vulkan::Context& v3d::Engine::getVulkanContext() const
//{
//	// @todo: Handle error
//	return *context;
//}

bool v3d::Engine::loadPreference()
{
	// buffer
	wchar_t documentsPath[MAX_PATH];

	// Get ducments path
	auto result = SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documentsPath);

	// Check result
	if (result != S_OK)
	{
		v3d::Logger::getInstance().critical("Failed to get My Documents folder path");
		return false;
	}

	std::wstring pathStr(documentsPath);
	//v3d::Logger::getInstance().info(L"My Documents: " + pathStr);

	return true;
}

bool v3d::Engine::initWindow(const std::string_view windowTitle)
{
	auto& logger = Logger::getInstance();
	window = new (std::nothrow) v3d::glfw::Window();
	if (window == nullptr) { logger.bad_alloc<v3d::glfw::Window>(); return false; }
	if (!window->initGLFW()) { logger.critical("Failed to initialize GLFW"); return false; }
	if (!window->initWindow(windowTitle)) { logger.critical("Failed to create GLFW window"); return false; }
	return true;
}

bool v3d::Engine::initContext()
{
	//context = new (std::nothrow) v3d::vulkan::Context();
	//if(context == nullptr ) { logger.bad_alloc<v3d::vulkan::Context>(); return false; }
	//if( !context->init( *window ) ) { logger.critical( "Failed to initialize Context" ); return false; }
	return true;
}

void v3d::Engine::release()
{
	// release vulkan first
	//if( context ) { delete context; context = nullptr; }
	// release glfw
	if (window) { delete window; window = nullptr; }
}
