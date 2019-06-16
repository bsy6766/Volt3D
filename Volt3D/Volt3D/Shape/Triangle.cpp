/**
*	@file Triangle.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Triangle.h"

#include "Utils/Logger.h"
#include "Utils/Utility.h"

v3d::Triangle::Triangle()
	: p1(0.0f)
	, p2(0.0f)
	, p3(0.0f)
	, n(0.0f)
{}

v3d::Triangle::Triangle(const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3) 
	: p1(p1)
	, p2(p2)
	, p3(p3)
	, n(v3d::Utility::Math3D::getNormal(p1, p2, p3))
{}

v3d::Triangle::Triangle(const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3, const glm::vec3& n)
	: p1(p1)
	, p2(p2)
	, p3(p3)
	, n(n)
{
	//checkOrientation();
}

v3d::Triangle::~Triangle() {}

/*
void v3d::Triangle::checkOrientation()
{
	int total = 0;
	int index = 0;
	std::vector<glm::vec3> vertices = { p1, p2, p3 };
	auto it = vertices.begin();
	auto next = vertices.begin();
	std::advance(next, 1);

	for (; next != vertices.end(); )
	{
		total += static_cast<int>((next->x - it->x) * (next->y + it->y));
		index++;
		it++;
		next++;

		if (next == vertices.end())
		{
			next = vertices.begin();
			total += static_cast<int>((next->x - it->x) * (next->y + it->y));
			break;
		}
	}

	if (total >= 0)
	{
		// clock wise
		auto vert_it = std::begin(vertices);
		std::advance(vert_it, 1);
		std::reverse(vert_it, std::end(vertices));

		p1 = vertices.at(0);
		p2 = vertices.at(1);
		p3 = vertices.at(2);

		n = v3d::Utility::Math3D::getNormal(p1, p2, p3);
	}
	else
	{
		// counter clock wise
		return;
	}
}
*/

void v3d::Triangle::print() const
{
	v3d::Logger& logger = v3d::Logger::getInstance();

	logger.info("[Triangle] Info");
	logger.info("p1: " + glm::to_string(p1));
	logger.info("p2: " + glm::to_string(p2));
	logger.info("p3: " + glm::to_string(p3));
	logger.info("n: " + glm::to_string(n));
}

