/**
*	@file Sphere.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Sphere.h"

#include "Utils/Logger.h"

v3d::Sphere::Sphere()
	: radius(0)
	, position(0)
{}

v3d::Sphere::Sphere(const float radius, const glm::vec3 & position)
	: radius(radius)
	, position(position)
{
	if (radius < 0) this->radius = 0.0f;
}

v3d::Sphere::~Sphere() {}

float v3d::Sphere::getDiameter() const
{
	return radius * 2.0f;
}

void v3d::Sphere::print() const
{
	v3d::Logger& logger = v3d::Logger::getInstance();

	logger.info("[Sphere] Info");
	logger.info("position: " + glm::to_string(position));
	logger.info("radius: " + std::to_string(radius));
}
