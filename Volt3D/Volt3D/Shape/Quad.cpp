/**
*	@file Plane.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Quad.h"

#include "Utils/Logger.h"

v3d::Quad::Quad()
	: p1(0.0f)
	, p2(0.0f)
	, p3(0.0f)
	, p4(0.0f)
{}

v3d::Quad::Quad(const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3, const glm::vec3 & p4)
	: p1(p1)
	, p2(p2)
	, p3(p3)
	, p4(p4)
{}

v3d::Quad::~Quad() {}

void v3d::Quad::print() const
{
	v3d::Logger& logger = v3d::Logger::getInstance();

	logger.info("[Quad] Info");
	logger.info("p1: " + glm::to_string(p1));
	logger.info("p2: " + glm::to_string(p2));
	logger.info("p3: " + glm::to_string(p3));
	logger.info("p4: " + glm::to_string(p4));
}
