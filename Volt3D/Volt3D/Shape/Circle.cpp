/**
*	@file Circle.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Circle.h"

#include "Utils/Logger.h"

v3d::Circle::Circle()
	: position(0.0f)
	, radius(0.0f)
{}

v3d::Circle::Circle(const glm::vec2 & position, const float radius)
	: position(position)
	, radius(radius)
{}

v3d::Circle::~Circle() {}

inline float v3d::Circle::getDiameter() const
{
	return radius + radius;
}

void v3d::Circle::print() const
{
	v3d::Logger::getInstance().info("[Circle] Info");
	v3d::Logger::getInstance().info("Position: " + glm::to_string(position));
	v3d::Logger::getInstance().info("Radius: " + std::to_string(radius));
}
