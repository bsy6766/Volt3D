/**
*	@file Time.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Time.h"

#include "Engine.h"
#include "Config/BuildConfig.h"

V3D_NS_BEGIN

Time::Time()
	: currentTime( 0 )
	, previousTime( 0 )
	, elapsedTime( 0 )
	, fps( 0 )
	, fpsElapsedTime( 0 )
	, onFPSUpdated( nullptr )
{}

v3d::Time* Time::get() { return v3d::Engine::get()->getTime(); }

void Time::updateTime()
{
	previousTime = currentTime;
	currentTime = glfwGetTime();

	elapsedTime = currentTime - previousTime;

	updateFPS();
}

void Time::updateFPS()
{
	fpsElapsedTime += elapsedTime;
	fps++;

	if (fpsElapsedTime > 1.0)
	{
		if (onFPSUpdated) onFPSUpdated( fps );

#ifdef BUILD_DEBUG
		Logger::getInstance().traceConsole( "FPS: " + std::to_string( fps ) );
#endif

		fps = 0;
		fpsElapsedTime -= 1.0;
	}
}

void Time::reset()
{
	currentTime = 0.0;
	previousTime = 0.0;
	elapsedTime = 0.0;
	fps = 0;
	fpsElapsedTime = 0.0;
}

int Time::getFPS() const
{
	return fps;
}

double Time::getElaspedTime() const
{
	return elapsedTime;
}

double Time::getCurrentTime() const
{
	return currentTime;
}

void Time::setOnFPSUpdateCallback( const std::function<void( const int )>* func )
{
	if (func == nullptr)	onFPSUpdated = nullptr;
	else					onFPSUpdated = *func;
}

V3D_NS_END