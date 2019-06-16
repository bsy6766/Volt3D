/**
*	@file AABB.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "AABB.h"

#include "Utils/Logger.h"

v3d::AABB::AABB()
	: position(0.0f)
	, halfExtents(0.0f)
{}

v3d::AABB::AABB(const glm::vec3 & position, const glm::vec3 & halfExtents)
	: position(position)
	, halfExtents(halfExtents)
{}

v3d::AABB::~AABB() {}

glm::vec3 v3d::AABB::getMin() const
{
	return position - halfExtents;
}

glm::vec3 v3d::AABB::getMax() const
{
	return position + halfExtents;
}

std::array<glm::vec3, 8> v3d::AABB::getPoints() const
{
	std::array<glm::vec3, 8> ret;

	const glm::vec3& min = getMin();
	const glm::vec3& max = getMax();

	// Front bottom left
	ret[0].x = min.x;
	ret[0].y = min.y;
	ret[0].z = min.z;

	// Front top left
	ret[1].x = min.x;
	ret[1].y = max.y;
	ret[1].z = min.z;

	// Front top right
	ret[2].x = max.x;
	ret[2].y = max.y;
	ret[2].z = min.z;

	// Front bottom right
	ret[3].x = max.x;
	ret[3].y = min.y;
	ret[3].z = min.z;

	// Back bottom left
	ret[4].x = min.x;
	ret[4].y = min.y;
	ret[4].z = max.z;

	// Front top left
	ret[5].x = min.x;
	ret[5].y = max.y;
	ret[5].z = max.z;

	// Front top right
	ret[6].x = max.x;
	ret[6].y = max.y;
	ret[6].z = max.z;

	// Front bottom right
	ret[7].x = max.x;
	ret[7].y = min.y;
	ret[7].z = max.z;

	// Done.
	return ret;
}

void v3d::AABB::print() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.info("[AABB] info");
	logger.info("Position: " + glm::to_string(position));
	logger.info("Half Extents: " + glm::to_string(halfExtents));
}