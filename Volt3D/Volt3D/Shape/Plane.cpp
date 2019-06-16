/**
*	@file Plane.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Plane.h"

#include "Triangle.h"

#include "Utils/Logger.h"

v3d::Plane::Plane()
	: normal(0.0f)
	, distanceToOrigin(0.0f)
{}

v3d::Plane::Plane(const glm::vec3 & normal, const float distanceToOrigin)
	: normal(glm::normalize(normal))
	, distanceToOrigin(distanceToOrigin)
{}

v3d::Plane::~Plane() {}

void v3d::Plane::set(const glm::vec3 & normal, const float distanceToOrigin)
{
	const float len = 1.0f / glm::length(normal);
	this->normal = normal * len;
	this->distanceToOrigin = distanceToOrigin * len;
}

void v3d::Plane::print() const
{
	v3d::Logger& logger = v3d::Logger::getInstance();

	logger.info("[Plane] Info");
	logger.info("normal: " + glm::to_string(normal));
	logger.info("distanceToOrigin: " + std::to_string(distanceToOrigin));
}
