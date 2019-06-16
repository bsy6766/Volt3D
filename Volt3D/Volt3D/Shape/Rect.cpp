/**
*	@file Rect.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Rect.h"

#include "AABB.h"

#include "Utils/Logger.h"

v3d::Rect::Rect()
	: min(0)
	, max(0)
{}

v3d::Rect::Rect(const glm::vec2 & min, const glm::vec2 & max)
	: min(min)
	, max(max)
{}

v3d::Rect::Rect(const v3d::AABB & aabb)
	: min(aabb.getMin())
	, max(aabb.getMax())
{}

v3d::Rect::~Rect() {}

void v3d::Rect::set(const glm::vec2 & center, const glm::vec2 & size)
{
	min = center - (size * 0.5f);
	max = center + (size * 0.5f);
}

glm::vec2 v3d::Rect::getSize() const
{
	return max - min;
}

glm::vec2 v3d::Rect::getCenter() const
{
	return min + (max - min);
}

std::array<glm::vec2, 4> v3d::Rect::getPoints() const
{
	return { min, glm::vec2(min.x, max.y), max, glm::vec2(max.x, min.y) };
}

/*
void v3d::Rect::merge(const v3d::Rect & other)
{
	auto thisMin = getMin();
	auto thisMax = getMax();

	auto otherMin = other.getMin();
	auto otherMax = other.getMax();

	auto newMin = glm::vec2(glm::min(thisMin.x, otherMin.x), glm::min(thisMin.y, otherMin.y));
	auto newMax = glm::vec2(glm::max(thisMax.x, otherMax.x), glm::max(thisMax.y, otherMax.y));

	this->size = newMax - newMin;
	this->center = newMin + (size * 0.5f);
}

bool v3d::Rect::containsPoint(const glm::vec2 & point) const
{
	auto min = getMin();
	auto max = getMax();
	return ((min.x <= point.x && point.x <= max.x) && (min.y <= point.y && point.y <= max.y));
}

bool v3d::Rect::doesIntersectsWith(const Rect & other) const
{
	auto min = getMin();
	auto max = getMax();
	auto oMin = other.getMin();
	auto oMax = other.getMax();

	return (min.x < oMax.x && max.x > oMin.x) &&
		(min.y < oMax.y && max.y > oMin.y);
}

bool v3d::Rect::doesIntersectsWith(const v3d::Quad2D & other) const
{
	return containsPoint(other.p1) || containsPoint(other.p2) || containsPoint(other.p3) || containsPoint(other.p4);
}

v3d::Rect v3d::Rect::getIntersectingRect(const Rect & other) const
{
	// Get min and max
	auto aMin = getMin();
	auto aMax = getMax();

	auto bMin = other.getMin();
	auto bMax = other.getMax();

	// Get intersection origin
	auto iMin = glm::vec2(0);
	iMin.x = (aMin.x > bMin.x) ? aMin.x : bMin.x;
	iMin.y = (aMin.y > bMin.y) ? aMin.y : bMin.y;

	// get Size
	auto iSize = glm::vec2(0);

	if (aMax.x < bMax.x)
	{
		iSize.x = aMax.x - iMin.x;
	}
	else
	{
		iSize.x = bMax.x - iMin.x;
	}

	if (aMax.y < bMax.y)
	{
		iSize.y = aMax.y - iMin.y;
	}
	else
	{
		iSize.y = bMax.y - iMin.y;
	}

	return v3d::Rect(iMin + (iSize * 0.5f), iSize);
}
*/

void v3d::Rect::print() const
{
	v3d::Logger& logger = v3d::Logger::getInstance();

	logger.info("[Rect] Info");
	logger.info("Min: " + glm::to_string(min));
	logger.info("Max: " + glm::to_string(max));
}

