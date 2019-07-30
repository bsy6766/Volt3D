﻿/**
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
#include "Vulkan/Devices.h"
#include "Texture/TextureManager.h"

v3d::Engine* v3d::Engine::instance = nullptr;

V3D_NS_BEGIN

Engine::Engine()
	: window( nullptr )
	, time( nullptr )
	, context( nullptr )
	, director( nullptr )
	, preference( nullptr )
	, inputManager( nullptr )
	, textureManager( nullptr )
{
	v3d::Logger::getInstance().init( FileSystem::getWorkingDirectoryW(), L"log.txt" );
	v3d::Logger::getInstance().initConsole();
	v3d::Engine::instance = this;
}

Engine::~Engine()
{
	release();
}

bool Engine::init( const char* windowTitle, const std::wstring& folderName )
{
	if (!loadPreference( folderName )) return false;

	time = new v3d::Time();
	inputManager = new v3d::InputManager();
	if (!initWindow( windowTitle )) return false;
	if (!initContext()) return false;
	director = new v3d::Director( *inputManager );
	if (!initTextureManager()) return false;

	return true;
}

bool Engine::loadPreference( const std::wstring& folderName )
{
	preference = new v3d::Preference();

	return preference->init( folderName );
}

bool Engine::initWindow( const char* windowTitle )
{
	auto& logger = Logger::getInstance();
	window = new v3d::glfw::Window( *inputManager );
	if (!window->initGLFW()) { logger.critical( "Failed to initialize GLFW" ); return false; }
	if (!window->initWindow( windowTitle, preference->getInt( "display_resolution_width" ), preference->getInt( "display_resolution_height" ), toWindowModeEnum( preference->getInt( "display_window_mode" ) ) )) { logger.critical( "Failed to create GLFW window" ); return false; }
	return true;
}

bool Engine::initContext()
{
	context = new v3d::vulkan::Context( *window );
	if (!context->init( *window, true )) { v3d::Logger::getInstance().critical( "Failed to initialize Context" ); return false; }
	return true;
}

bool Engine::initTextureManager()
{
	auto devices = context->getDevices();
	if (devices == nullptr) return false;
	// @todo: handle all types of textures
	textureManager = std::shared_ptr<v3d::TextureManager>( new v3d::TextureManager( devices->getProperties().limits.maxImageDimension2D ) );
	return true;
}

void Engine::release()
{
	// release vulkan first
	textureManager->clear();
	textureManager->print();
	textureManager = nullptr;
	SAFE_DELETE( context );
	SAFE_DELETE( window );
	SAFE_DELETE( time );
	SAFE_DELETE( director );
	SAFE_DELETE( inputManager );
	if (preference) preference->save();
	SAFE_DELETE( preference );
}

void Engine::run()
{
	time->resetTime();

	static glm::uvec2 fb = window->getFrameBufferSize();

	while (window && window->isRunning())
	{
		window->pollGLFWEvent();

		time->updateTime();
		const float delta = static_cast<float>(time->getElaspedTime());

		preUpdate( delta );
		update( delta );

		if (!window->isIconified()) render();
		postUpdate( delta );
	}

	context->waitIdle();
}

void Engine::end()
{
	if (window) window->closeWindow();
}

void Engine::preUpdate( const float delta )
{
}

void Engine::update( const float delta )
{
	if (inputManager->isKeyPressed( v3d::KeyCode::eA, true )) v3d::Logger::getInstance().info( "A pressed!" );
	if (director) director->update( delta );
}

void Engine::postUpdate( const float delta )
{
	inputManager->postUpdate();
}

void Engine::render()
{
	if (!context) return;
	context->render();
}

v3d::glfw::Window* Engine::getWindow() const
{
	return window;
}

v3d::Time* Engine::getTime() const
{
	return time;
}

v3d::vulkan::Context* Engine::getVulkanContext() const
{
	return context;
}

v3d::Director* Engine::getDirector() const
{
	return director;
}

v3d::InputManager* Engine::getInputManager() const
{
	return inputManager;
}

V3D_NS_END