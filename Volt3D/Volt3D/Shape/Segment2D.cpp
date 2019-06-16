/**
*	@file Segment2D.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Segment2D.h"

#include "Utils/Logger.h"

v3d::Segment2D::Segment2D()
	: p1(0.0f)
	, p2(0.0f)
{}

v3d::Segment2D::Segment2D(const glm::vec2 & p1, const glm::vec2 & p2)
	: p1(p1)
	, p2(p2)
{}

v3d::Segment2D::~Segment2D() {}

float v3d::Segment2D::getLength() const
{
	return glm::length(p2 - p1);
}

glm::vec2 v3d::Segment2D::getClosestPoint(const glm::vec2 & point, float& t)
{
	// p1 to p2
	glm::vec2 p12 = p2 - p1;

	// Project point to p12
	t = glm::dot(point - p1, p12);

	if (t <= 0.0f)
	{
		// point projects outside of line Segment, on the p1 side.
		t = 0.0f;
		return p1;
	}
	else
	{
		// Always gives positive value because ||p12||^2
		float denom = glm::dot(p12, p12);

		if (t >= denom)
		{
			// point projects outside of line Segment, on the p2 side
			t = 1.0f;
			return p2;
		}
		else
		{
			// point projects inside the line Segment. 
			t = t / denom;
			return p1 + (t * p12);
		}
	}
}

void v3d::Segment2D::print() const
{
	v3d::Logger::getInstance().info("[Segment] Info");
	v3d::Logger::getInstance().info("First point: " + glm::to_string(p1));
	v3d::Logger::getInstance().info("Second point: " + glm::to_string(p2));
}
