/**
*	@file OrientedRect.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "OrientedRect.h"

#include "Utils/Logger.h"

v3d::OrientedRect::OrientedRect()
	: position(0.0f)
	, halfExtents(0.0f)
	, rotation(0.0f)
{}

v3d::OrientedRect::OrientedRect(const glm::vec2 & position, const glm::vec2 & halfExtents, const float rotation)
	: position(position)
	, halfExtents(halfExtents)
	, rotation(rotation)
{}

v3d::OrientedRect::~OrientedRect() {}

std::array<glm::vec2, 4> v3d::OrientedRect::getPoints() const
{
	const glm::mat4 rotMat = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)), glm::radians(-rotation), glm::vec3(0.0f, 0.0f, 1.0f));

	std::array<glm::vec2, 4> points = { -halfExtents, glm::vec2(-halfExtents.x, halfExtents.y), halfExtents, glm::vec2(halfExtents.x, -halfExtents.y) };

	for (auto& point : points)
	{
		point = glm::vec2(rotMat * glm::vec4(point, 0.0f, 1.0f));
	}

	return points;
}

void v3d::OrientedRect::print() const
{
	v3d::Logger::getInstance().info("[OrientedRect] Info");
	v3d::Logger::getInstance().info("Position: ( " + std::to_string(position.x) + ", " + std::to_string(position.y) + ")");
	v3d::Logger::getInstance().info("Half Extents: ( " + std::to_string(halfExtents.x) + ", " + std::to_string(halfExtents.y) + ")");
}
