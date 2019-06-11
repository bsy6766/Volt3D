/**
*	@file Color3F.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Color3F.h"

#include "Color3B.h"
#include "Color4B.h"
#include "Color4F.h"

const v3d::Color3F v3d::Color3F::WHITE  ( 1.0f,  1.0f,  1.0f);
const v3d::Color3F v3d::Color3F::YELLOW ( 1.0f,  1.0f,  0.0f);
const v3d::Color3F v3d::Color3F::GREEN  ( 0.0f,  1.0f,  0.0f);
const v3d::Color3F v3d::Color3F::BLUE   ( 0.0f,  0.0f,  1.0f);
const v3d::Color3F v3d::Color3F::RED    ( 1.0f,  0.0f,  0.0f);
const v3d::Color3F v3d::Color3F::MAGENTA( 1.0f,  0.0f,  1.0f);
const v3d::Color3F v3d::Color3F::BLACK  ( 0.0f,  0.0f,  0.0f);
const v3d::Color3F v3d::Color3F::ORANGE ( 1.0f,  0.5f,  0.0f);
const v3d::Color3F v3d::Color3F::GRAY   (0.65f, 0.65f, 0.65f);

v3d::Color3F::Color3F()
	: r(0.0f)
	, g(0.0f)
	, b(0.0f)
{}

v3d::Color3F::Color3F(const float r, const float g, const float b)
	: r(glm::clamp(r, 0.0f, 1.0f))
	, g(glm::clamp(g, 0.0f, 1.0f))
	, b(glm::clamp(b, 0.0f, 1.0f))
{}

v3d::Color3F::Color3F(const Color3B& other)
{
	r = static_cast<float>(other.r) / 255.0f;
	g = static_cast<float>(other.g) / 255.0f;
	b = static_cast<float>(other.b) / 255.0f;
}

v3d::Color3F::Color3F(const Color4B& other)
{
	r = static_cast<float>(other.r) / 255.0f;
	g = static_cast<float>(other.g) / 255.0f;
	b = static_cast<float>(other.b) / 255.0f;
}

v3d::Color3F::Color3F(const Color4F& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
}

v3d::Color3F::Color3F(const glm::vec3 & color) 
	: r(glm::clamp(color.r, 0.0f, 1.0f))
	, g(glm::clamp(color.g, 0.0f, 1.0f))
	, b(glm::clamp(color.b, 0.0f, 1.0f))
{}

bool v3d::Color3F::operator==(const Color3B& right) const
{
	return (v3d::Color3B(*this) == right);
}

bool v3d::Color3F::operator==(const Color3F& right) const
{
	return (r == right.r && g == right.g && b == right.b);
}

bool v3d::Color3F::operator==(const Color4B& right) const
{
	return (255 == right.a && v3d::Color4B(*this) == right);
}

bool v3d::Color3F::operator==(const Color4F& right) const
{
	return (r == right.r && g == right.g && b == right.b && 1.0f == right.a);
}

bool v3d::Color3F::operator!=(const Color3B& right) const
{
    return !(*this == right);
}

bool v3d::Color3F::operator!=(const Color3F& right) const
{
    return !(*this == right);
}

bool v3d::Color3F::operator!=(const Color4B& right) const
{
    return !(*this == right);
}

bool v3d::Color3F::operator!=(const Color4F& right) const
{
    return !(*this == right);
}

v3d::Color3F v3d::Color3F::operator+(const Color3F & right) const
{
	return Color3F(r + right.r, g + right.g, b + right.b);
}

v3d::Color3F v3d::Color3F::operator-(const Color3F & right) const
{
	return Color3F(r - right.r, g - right.g, b - right.b);
}

v3d::Color3F v3d::Color3F::operator*(const float right) const
{
	return Color3F(r * right, g * right, b * right);
}

v3d::Color3F v3d::Color3F::operator/(const float right) const
{
	return Color3F(r / right, g / right, b / right);
}

void v3d::Color3F::set(const glm::vec3 & color)
{
	r = glm::clamp(color.r, 0.0f, 1.0f);
	g = glm::clamp(color.g, 0.0f, 1.0f);
	b = glm::clamp(color.b, 0.0f, 1.0f);
}

void v3d::Color3F::setR(const float r)
{
	this->r = r;
}

void v3d::Color3F::setG(const float g)
{
	this->g = g;
}

void v3d::Color3F::setB(const float b)
{
	this->b = b;
}

glm::vec3 v3d::Color3F::toVec3() const
{
	return glm::vec3(r, g, b);
}