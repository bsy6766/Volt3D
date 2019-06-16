/**
*	@file OBB.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "OBB.h"

#include "Utils/Logger.h"

v3d::OBB::OBB()
	: position(0.0f)
	, halfExtents(0.0f)
	, orientation(1.0f)
{}

v3d::OBB::OBB(const glm::vec3 & position, const glm::vec3 & halfExtents)
	: position(position)
	, halfExtents(halfExtents)
	, orientation(1.0f)
{}

v3d::OBB::OBB(const glm::vec3 & position, const glm::vec3 & halfExtents, const glm::mat3 & orientation)
	: position(position)
	, halfExtents(halfExtents)
	, orientation(orientation)
{}

v3d::OBB::~OBB() {}

void v3d::OBB::print() const
{
	v3d::Logger& logger = v3d::Logger::getInstance();

	logger.info("[OBB] Info");
	logger.info("Position: " + glm::to_string(position));
	logger.info("Half Extents: " + glm::to_string(halfExtents));
	logger.info("Rotation: " + glm::to_string(orientation));
}
