/**
*	@file Triangle2D.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Triangle2D.h"

#include "Utils/Logger.h"

v3d::Triangle2D::Triangle2D()
	: p1(0.0f)
	, p2(0.0f)
	, p3(0.0f)
{}

v3d::Triangle2D::Triangle2D(const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3)
	: p1(p1)
	, p2(p2)
	, p3(p3)
{}

v3d::Triangle2D::~Triangle2D() {}

void v3d::Triangle2D::print() const
{
	v3d::Logger& logger = v3d::Logger::getInstance();

	logger.info("[Triangle2D] Info");
	logger.info("p1: " + glm::to_string(p1));
	logger.info("p2: " + glm::to_string(p2));
	logger.info("p3: " + glm::to_string(p3));
}

