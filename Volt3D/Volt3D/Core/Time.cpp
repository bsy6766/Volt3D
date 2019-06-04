/**
*	@file Time.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Time.h"

v3d::glfw::Time::Time()
	: currentTime(0)
	, previousTime(0)
	, elapsedTime(0)
	, fps(0)
	, fpsElapsedTime(0)
	, onFPSUpdated(nullptr)
{}

void v3d::glfw::Time::updateTime()
{
	previousTime = currentTime;
	currentTime = glfwGetTime();

	elapsedTime = currentTime - previousTime;
}

void v3d::glfw::Time::updateFPS()
{
	fpsElapsedTime += elapsedTime;
	fps++;

	if (fpsElapsedTime > 1.0)
	{
		if (onFPSUpdated) onFPSUpdated(fps);

		fps = 0;
		fpsElapsedTime -= 1.0;
	}
}
void v3d::glfw::Time::resetTime()
{
	currentTime = 0.0;
	previousTime = 0.0;
	elapsedTime = 0.0;
	fps = 0;
	fpsElapsedTime = 0.0;
}

int v3d::glfw::Time::getFPS() const
{
	return fps;
}

double v3d::glfw::Time::getElaspedTime() const
{
	return elapsedTime;
}

double v3d::glfw::Time::getCurrentTime() const
{
	return currentTime;
}

void v3d::glfw::Time::setOnFPSUpdateCallback(const std::function<void(const int)>* func)
{
	if (func == nullptr)	onFPSUpdated = nullptr;
	else					onFPSUpdated = *func;
}