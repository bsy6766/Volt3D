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
#include "Shader/Shader.h"
#include "Resource/Resource.h"

#include "Config/BuildConfig.h"

v3d::Engine* v3d::Engine::instance = nullptr;

V3D_NS_BEGIN

Engine::Engine()
	: window( nullptr )
	, time( nullptr )
	, context( nullptr )
	, director( nullptr )
	, preference( nullptr )
	, inputManager( nullptr )
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
	// Load preference first.
	if (!loadPreference( folderName )) return false;

	// ====================
	// Initialize everything that needs to run the engine.
	initTime();
	if (!initInputManager()) return false;
	if (!initResource()) return false;
	if (!initWindow( windowTitle )) return false;
	if (!initContext()) return false;
	if (!initDefaultShaders()) return false;
	context->initGraphics();	// @note: This should be renderer...
	// ====================

	// Create director.
	director = new v3d::Director( *inputManager );

	return true;
}

bool Engine::loadPreference( const std::wstring& folderName )
{
	preference = new v3d::Preference();
	return preference->init( folderName );
}

void Engine::initTime()
{
	time = new v3d::Time();
	time->reset();
}

bool Engine::initInputManager()
{
	inputManager = std::unique_ptr<v3d::InputManager>(new v3d::InputManager());
	return inputManager->init();
}

bool Engine::initResource()
{
	return v3d::Resource::init();
}

bool Engine::initDefaultShaders()
{
	glslang::InitializeProcess();

	//std::shared_ptr<v3d::Shader> vertShader = v3d::Resource::load<v3d::Shader>( "V3D.DEFAULT.3D.VERT", "Shaders/vert.vert" );

//	v3d::Shader* vertShader = v3d::Shader::create( "V3D.DEFAULT.3D.VERT", "Shaders/vert.vert" );
//	if (!vertShader) return false;
//	if (!shaderCache->addShader(std::shared_ptr<v3d::Shader>(vertShader))) return false;
//
//	v3d::Shader* fragShader = v3d::Shader::create( "V3D.DEFAULT.3D.FRAG", "Shaders/frag.frag" );
//	if (!fragShader) return false;
//	if (!shaderCache->addShader( std::shared_ptr<v3d::Shader>( fragShader ) )) return false;
//
//#ifdef BUILD_DEBUG
//	v3d::Logger::getInstance().info( "Initialized {} default shaders.", shaderCache->count() );
//#endif

	return true;
}

bool Engine::initWindow( const char* windowTitle )
{
	auto& logger = Logger::getInstance();
	window = new v3d::glfw::Window();
	if (!window->initGLFW()) { logger.critical( "Failed to initialize GLFW" ); return false; }
	if (!window->initWindow( windowTitle, preference->getInt( "display_resolution_width" ), preference->getInt( "display_resolution_height" ), toWindowModeEnum( preference->getInt( "display_window_mode" ) ) )) { logger.critical( "Failed to create GLFW window" ); return false; }
	return true;
}

bool Engine::initContext()
{
	context = new v3d::vulkan::Context();
	if (!context->init( true )) { v3d::Logger::getInstance().critical( "Failed to initialize Context" ); return false; }
	return true;
}

void Engine::release()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info( "[Engine] Releasing..." );
#endif
	// release vulkan first
	SAFE_DELETE( context );
	SAFE_DELETE( window );
	SAFE_DELETE( time );
	SAFE_DELETE( director );
	inputManager.reset();
	if (preference) preference->save();
	SAFE_DELETE( preference );

	glslang::FinalizeProcess();
}

void Engine::run()
{
	time->reset();

	static glm::uvec2 fb = window->getFrameBufferSize();

	while (window && window->isRunning())
	{
		preUpdate();
		const float delta = static_cast<float>(time->getElaspedTime());
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

void Engine::preUpdate()
{
	if(window) window->pollGLFWEvent();
	if(time) time->updateTime();
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

inline v3d::glfw::Window* Engine::getWindow() const
{
	return window;
}

inline v3d::Time* Engine::getTime() const
{
	return time;
}

inline v3d::vulkan::Context* Engine::getVulkanContext() const
{
	return context;
}

inline v3d::Director* Engine::getDirector() const
{
	return director;
}

inline v3d::InputManager& Engine::getInputManager() const
{
	return *inputManager.get();
}

V3D_NS_END