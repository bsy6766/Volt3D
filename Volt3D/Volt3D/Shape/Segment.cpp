/**
*	@file Segment.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Segment.h"

#include "Utils/Logger.h"

v3d::Segment::Segment()
	: p1(0.0f)
	, p2(0.0f)
{}

v3d::Segment::Segment(const glm::vec3 & p1, const glm::vec3 & p2)
	: p1(p1)
	, p2(p2)
{}

v3d::Segment::~Segment() {}

float v3d::Segment::getLength() const
{
	return glm::length(p2 - p1);
}

void v3d::Segment::print() const
{
	v3d::Logger::getInstance().info("[Segment] Info");
	v3d::Logger::getInstance().info("First point: " + glm::to_string(p1));
	v3d::Logger::getInstance().info("Second point: " + glm::to_string(p2));
}
