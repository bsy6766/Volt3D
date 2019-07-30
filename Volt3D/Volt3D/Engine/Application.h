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

V3D_NS_BEGIN

// Foward declaration
class Engine;

/**
*	@class Application
*	@brief The application of engine
*	@note Singleton design
*
*	@group Engine
*
*	@since 1.0
*/
class VOLT3D_DLL Application
{
protected:
	Application();

	std::wstring name;
	v3d::Engine* engine;

public:
	virtual ~Application();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Application );
	DEFAULT_MOVE_CONSTRUCTORS( Application );

	/** 
	*	Get application instance
	*	@note Thread safe singleton design (C++11)
	*/
	static v3d::Application& get()
	{
		static v3d::Application instance;
		return instance;
	}

	/**
	*	Initialize the application.
	*	@param appName The name of application.
	*	@param windowTitle A window title for GLFW window
	*/
	virtual bool init( const wchar_t* appName, const char* windowTitle );

	/**	Run the application. */
	virtual void run();

	/** Get application name. */
	std::wstring getName() const { return name; }

	/** Get engine. */
	v3d::Engine* getEngine() const { return engine; }
};

 V3D_NS_END

#endif