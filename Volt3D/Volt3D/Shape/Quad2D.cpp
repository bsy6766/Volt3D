/**
*	@file Quad2D.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Quad2D.h"

#include "Utils/Logger.h"

//#include "Volt3D/Physics/CollisionDetection.h"

#include "Triangle2D.h"
#include "Rect.h"

v3d::Quad2D::Quad2D()
	: p1(0.0f)
	, p2(0.0f)
	, p3(0.0f)
	, p4(0.0f)
{}

v3d::Quad2D::Quad2D(const glm::vec2 & p1, const glm::vec2 & p2, const glm::vec2 & p3, const glm::vec2 & p4)
	: p1(p1)
	, p2(p2)
	, p3(p3)
	, p4(p4)
{}

v3d::Quad2D::Quad2D(const v3d::Rect & rect)
	: p1(0.0f)
	, p2(0.0f)
	, p3(0.0f)
	, p4(0.0f)
{
	set(rect);
}

v3d::Quad2D::~Quad2D() {}

void v3d::Quad2D::set(const v3d::Rect & rect)
{
	p1 = rect.min;

	p2.x = rect.min.x;
	p2.y = rect.max.y;

	p3 = rect.max;

	p4.x = rect.max.x;
	p4.y = rect.min.y;
}

v3d::Rect v3d::Quad2D::toRect() const
{
	// get min max
	float minX = glm::min(glm::min(glm::min(p1.x, p2.x), p3.x), p4.x);
	float minY = glm::min(glm::min(glm::min(p1.y, p2.y), p3.y), p4.y);
	float maxX = glm::max(glm::max(glm::max(p1.x, p2.x), p3.x), p4.x);
	float maxY = glm::max(glm::max(glm::max(p1.y, p2.y), p3.y), p4.y);

	glm::vec2 min(minX, minY);
	glm::vec2 max(maxX, maxY);
	glm::vec2 size = max - min;

	return v3d::Rect(min + (size * 0.5f), size);
}

/*
bool v3d::Quad2D::containsPoint(const glm::vec2 & point)
{
	// Simply make 2 triangles from quadrilateral and check if it contains point
	//return v3d::Triangle2D(p1, p2, p3).containsPoint(point) || v3d::Triangle2D(p3, p4, p1).containsPoint(point);

	return v3d::CollisionDetection::containsPoint(v3d::Triangle2D(p1, p2, p3), point) || v3d::CollisionDetection::containsPoint(v3d::Triangle2D(p3, p4, p1), point);
}

bool v3d::Quad2D::doesIntersects(const v3d::Quad2D & other)
{
	//std::vector<glm::vec2> pointsA = { p1, p2, p3, p4 };
	//std::vector<glm::vec2> pointsB = { other.p1, other.p2, other.p3, other.p4 };

	glm::vec2 pointsA[4] = { p1, p2, p3, p4 };
	glm::vec2  pointsB[4] = { other.p1, other.p2, other.p3, other.p4 };

	for (int p = 0; p < 2; p++)
	{
		const v3d::Quad2D& quad = (p == 0) ? *this : other;

		std::vector<glm::vec2> points = { quad.p1, quad.p2, quad.p3, quad.p4 };

		for (int i = 0; i < 4; ++i)
		{
			int mod = (i + 1) % 4;

			glm::vec2 p1 = points[i];
			glm::vec2 p2 = points[mod];

			glm::vec2 normal = glm::vec2(p2.y - p1.y, p1.x - p2.x);

			float minA = std::numeric_limits<float>::max();
			float maxA = std::numeric_limits<float>::min();

			for (auto& point : pointsA)
			{
				float projected = normal.x * point.x + normal.y * point.y;

				if (projected < minA)
				{
					minA = projected;
				}
				if (projected > maxA)
				{
					maxA = projected;
				}
			}

			float minB = std::numeric_limits<float>::max();
			float maxB = std::numeric_limits<float>::min();

			for (auto& point : pointsB)
			{
				float projected = normal.x * point.x + normal.y * point.y;

				if (projected < minB)
				{
					minB = projected;
				}
				if (projected > maxB)
				{
					maxB = projected;
				}
			}

			if (maxA < minB || maxB < minA)
				return false;
		}
	}	

	return true;
}
*/

void v3d::Quad2D::print()
{
	v3d::Logger& logger = v3d::Logger::getInstance();

	logger.info("[Quad2D] Info");
	logger.info("p1: " + glm::to_string(p1));
	logger.info("p2: " + glm::to_string(p2));
	logger.info("p3: " + glm::to_string(p3));
	logger.info("p4: " + glm::to_string(p4));
}

