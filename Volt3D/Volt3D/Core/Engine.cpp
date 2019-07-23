/**
*	@file Engine.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Engine.h"

#include "utils/FileSystem.h"
#include "utils/Logger.h"

#include "Time.h"
#include "Vulkan/Context.h"
#include "Window.h"
#include "Input/InputManager.h"
#include "Director.h"
#include "Preference.h"
#include "WindowMode.h"

v3d::Engine::Engine()
	: window(nullptr)
	, time(nullptr)
	, context(nullptr)
	, inputManager(nullptr)
	, director(nullptr)
	, preference(nullptr)
{
	v3d::Logger::getInstance().init(FileSystem::getWorkingDirectoryW(), L"log.txt");
	v3d::Logger::getInstance().initConsole();
}

v3d::Engine::~Engine()
{
	release();
}

bool v3d::Engine::init(const char* windowTitle, const std::wstring& folderName)
{
	if (!loadPreference(folderName)) return false;

	time = new v3d::glfw::Time();
	inputManager = new v3d::InputManager();
	if (!initWindow(windowTitle)) return false;
	if (!initContext()) return false;
	director = new v3d::Director(*inputManager);

	return true;
}

bool v3d::Engine::loadPreference(const std::wstring& folderName)
{
	preference = new v3d::Preference();

	return preference->init(folderName);
}

bool v3d::Engine::initWindow(const char* windowTitle)
{
	auto& logger = Logger::getInstance();
	window = new v3d::glfw::Window(*inputManager);
	if (!window->initGLFW()) { logger.critical("Failed to initialize GLFW"); return false; }
	if (!window->initWindow(windowTitle, preference->getInt("display_resolution_width"), preference->getInt("display_resolution_height"), toWindowModeEnum(preference->getInt("display_window_mode")))) { logger.critical("Failed to create GLFW window"); return false; }
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
	SAFE_DELETE(context);
	SAFE_DELETE(window);
	SAFE_DELETE(time);
	SAFE_DELETE(director);
	SAFE_DELETE(inputManager);
	if (preference) preference->save();
	SAFE_DELETE(preference);
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
	if (director) director->update(delta);
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

v3d::glfw::Window& v3d::Engine::getWindow() const
{
	// @todo: Handle error
	return *window;
}

v3d::vulkan::Context& v3d::Engine::getVulkanContext() const
{
	// @todo: Handle error
	return *context;
}

v3d::Director& v3d::Engine::getDirector() const
{
	return *director;
}

v3d::InputManager& v3d::Engine::getInputManager() const
{
	return *inputManager;
}
