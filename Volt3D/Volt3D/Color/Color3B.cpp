/**
*	@file Color3B.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Color3B.h"

#include "Color3F.h"
#include "Color4B.h"
#include "Color4F.h"

const v3d::Color3B v3d::Color3B::WHITE  (255, 255, 255);
const v3d::Color3B v3d::Color3B::RED    (255,   0,   0);
const v3d::Color3B v3d::Color3B::ORANGE (255, 127,   0);
const v3d::Color3B v3d::Color3B::YELLOW (255, 255,   0);
const v3d::Color3B v3d::Color3B::GREEN  (  0, 255,   0);
const v3d::Color3B v3d::Color3B::BLUE   (  0,   0, 255);
const v3d::Color3B v3d::Color3B::MAGENTA(255,   0, 255);
const v3d::Color3B v3d::Color3B::GRAY   (166, 166, 166);
const v3d::Color3B v3d::Color3B::BLACK  (  0,   0,   0);

v3d::Color3B::Color3B()
	: r(0)
	, g(0)
	, b(0)
{}

v3d::Color3B::Color3B(const unsigned char r, const unsigned char g, const unsigned char b)
	: r(r)
	, g(g)
	, b(b)
{}

v3d::Color3B::Color3B(const Color3F& other)
{
	r = static_cast<unsigned char>(other.r * 255.0f);
	g = static_cast<unsigned char>(other.g * 255.0f);
	b = static_cast<unsigned char>(other.b * 255.0f);
}

v3d::Color3B::Color3B(const Color4B& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
}

v3d::Color3B::Color3B(const Color4F& other)
{
	r = static_cast<unsigned char>(other.r * 255.0f);
	g = static_cast<unsigned char>(other.g * 255.0f);
	b = static_cast<unsigned char>(other.b * 255.0f);
}

bool v3d::Color3B::operator==(const Color3B& right) const
{
	return (r == right.r && g == right.g && b == right.b);
}

bool v3d::Color3B::operator==(const Color3F& right) const
{
	return (v3d::Color3F(*this) == right);
}

bool v3d::Color3B::operator==(const Color4B& right) const
{
	return (r == right.r && g == right.g && b == right.b && 255 == right.a);
}

bool v3d::Color3B::operator==(const Color4F& right) const
{
	return (1.0f == right.a && v3d::Color4F(*this) == right);
}

bool v3d::Color3B::operator!=(const Color3B& right) const
{
    return !(*this == right);
}

bool v3d::Color3B::operator!=(const Color3F& right) const
{
    return !(*this == right);
}

bool v3d::Color3B::operator!=(const Color4B& right) const
{
    return !(*this == right);
}

bool v3d::Color3B::operator!=(const Color4F& right) const
{
    return !(*this == right);
}

void v3d::Color3B::setR(const unsigned char r)
{
	this->r = glm::clamp(r, static_cast<unsigned char>(0), static_cast<unsigned char>(255));
}

void v3d::Color3B::setG(const unsigned char g)
{
	this->g = glm::clamp(g, static_cast<unsigned char>(0), static_cast<unsigned char>(255));
}

void v3d::Color3B::setB(const unsigned char b)
{
	this->b = glm::clamp(b, static_cast<unsigned char>(0), static_cast<unsigned char>(255));
}

glm::ivec3 v3d::Color3B::toIVec3() const
{
	return glm::ivec3(r, g, b);
}