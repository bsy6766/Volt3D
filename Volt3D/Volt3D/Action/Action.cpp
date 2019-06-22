/**
*	@file Action.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Action.h"

#include "Node/TransformNode.h"

#include "Config/BuildConfig.h"

v3d::Action::Action()
	: duration(0.0f)
	, elapsedTime(0.0f)
{}

v3d::Action::~Action() {}

float v3d::Action::getLinearEaseTime()
{
	if (elapsedTime == 0.0f)			return 0.0f;
	else if (elapsedTime > duration)	return 1.0f;
	else								return (elapsedTime / duration);
}

bool v3d::Action::setDuration(const float duration)
{
	if (duration < 0.0f) return false;
	this->duration = duration;
	return true;
}

void v3d::Action::updateEase(const float delta)
{
	if (delta == 0.0f) return;
	elapsedTime += delta;
}

bool v3d::Action::isDone() const
{
	if (duration == 0.0f)	return elapsedTime > 0.0f;
	else					return elapsedTime >= duration;
}

float v3d::Action::getExceededTime()
{
	if (elapsedTime >= duration)	return elapsedTime - duration;
	else							return 0.0f;
}

float v3d::Action::getDuration() const
{
	return duration;
}

float v3d::Action::getElapsedTime() const
{
	return elapsedTime;
}

bool v3d::Action::isInstant() const
{
	return duration == 0.0f;
}

void v3d::Action::reset()
{
	elapsedTime = 0.0f;
}
