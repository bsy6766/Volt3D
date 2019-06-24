/**
*	@file Engine.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Engine.h"

#include <WinBase.h>	// GetUserName
#include <Lmcons.h>		// UNLEN
#include <ShlObj.h>		// SHGetFolderPath

#include "utils/FileSystem.h"
#include "utils/Logger.h"

#include "Time.h"
#include "Vulkan/Context.h"
#include "Window.h"
#include "Input/InputManager.h"

v3d::Engine::Engine()
	: window(nullptr)
	, time(nullptr)
	, context(nullptr)
	, inputManager(nullptr)
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
	if (!loadPreference()) return false;
	if (!initWindow(windowTitle)) return false;
	if (!initContext()) return false;

	time = new v3d::glfw::Time();
	inputManager = new v3d::InputManager();

	return true;
}

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
	v3d::Logger::getInstance().info(L"My Documents: " + pathStr);

	return true;
}

bool v3d::Engine::initWindow(const std::string_view windowTitle)
{
	auto& logger = Logger::getInstance();
	window = new v3d::glfw::Window(*inputManager);
	if (!window->initGLFW()) { logger.critical("Failed to initialize GLFW"); return false; }
	if (!window->initWindow(windowTitle)) { logger.critical("Failed to create GLFW window"); return false; }
	return true;
}

bool v3d::Engine::initContext()
{
	context = new v3d::vulkan::Context(*window);
	if( !context->init( *window, true ) ) { v3d::Logger::getInstance().critical( "Failed to initialize Context" ); return false; }
	return true;
}

void v3d::Engine::release()
{
	// release vulkan first
	if( context ) { delete context; context = nullptr; }
	if (window) { delete window; window = nullptr; }
}

void v3d::Engine::run()
{
	time->resetTime();

	static glm::uvec2 fb = window->getFrameBufferSize();

	while (window && window->isRunning())
	{
		window->pollGLFWEvent();

		time->updateTime();
		const float delta = static_cast<float>(time->getElaspedTime());

		preUpdate(delta);
		update(delta);

		if(!window->isIconified()) render();
		postUpdate(delta);
	}

	context->waitIdle();
}

void v3d::Engine::end()
{
	if (window) window->closeWindow();
}

void v3d::Engine::preUpdate(const float delta)
{
}

void v3d::Engine::update(const float delta)
{
	if (inputManager->isKeyPressed(v3d::KeyCode::eA, true)) v3d::Logger::getInstance().info("A pressed!");
}

void v3d::Engine::postUpdate(const float delta)
{
	inputManager->postUpdate();
}

void v3d::Engine::render()
{
	if (!context) return;
	context->render();
}

v3d::glfw::Window& v3d::Engine::getView() const
{
	// @todo: Handle error
	return *window;
}

v3d::vulkan::Context& v3d::Engine::getVulkanContext() const
{
	// @todo: Handle error
	return *context;
}