/**
*	@file Color4F.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Color4F.h"

#include "Color3B.h"
#include "Color3F.h"
#include "Color4B.h"

const v3d::Color4F v3d::Color4F::WHITE  ( 1.0f,  1.0f,  1.0f,  1.0f);
const v3d::Color4F v3d::Color4F::YELLOW ( 1.0f,  1.0f,  0.0f,  1.0f);
const v3d::Color4F v3d::Color4F::GREEN  ( 0.0f,  1.0f,  0.0f,  1.0f);
const v3d::Color4F v3d::Color4F::BLUE   ( 0.0f,  0.0f,  1.0f,  1.0f);
const v3d::Color4F v3d::Color4F::RED    ( 1.0f,  0.0f,  0.0f,  1.0f);
const v3d::Color4F v3d::Color4F::MAGENTA( 1.0f,  0.0f,  1.0f,  1.0f);
const v3d::Color4F v3d::Color4F::BLACK  ( 0.0f,  0.0f,  0.0f,  1.0f);
const v3d::Color4F v3d::Color4F::ORANGE ( 1.0f,  0.5f,  0.0f,  1.0f);
const v3d::Color4F v3d::Color4F::GRAY   (0.65f, 0.65f, 0.65f,  1.0f);

v3d::Color4F::Color4F()
	: r(0.0f)
	, g(0.0f)
	, b(0.0f)
	, a(0.0f)
{}

v3d::Color4F::Color4F(const float r, const float g, const float b, const float a)
	: r(glm::clamp(r, 0.0f, 1.0f))
	, g(glm::clamp(g, 0.0f, 1.0f))
	, b(glm::clamp(b, 0.0f, 1.0f))
	, a(glm::clamp(a, 0.0f, 1.0f))
{}

v3d::Color4F::Color4F(const Color3B& other)
	: r(static_cast<float>(other.r) / 255.0f)
	, g(static_cast<float>(other.g) / 255.0f)
	, b(static_cast<float>(other.b) / 255.0f)
	, a(1.0f)
{}

v3d::Color4F::Color4F(const Color3F& other)
	: r(other.r)
	, g(other.g)
	, b(other.b)
	, a(1.0f)
{}

v3d::Color4F::Color4F(const Color3F & other, const float a)
	: r(other.r)
	, g(other.g)
	, b(other.b)
	, a(a)
{}

v3d::Color4F::Color4F(const Color4B& other)
{
	r = static_cast<float>(other.r) / 255.0f;
	g = static_cast<float>(other.g) / 255.0f;
	b = static_cast<float>(other.b) / 255.0f;
	a = static_cast<float>(other.a) / 255.0f;
}

v3d::Color4F::Color4F(const glm::vec3 & color)
	: r(glm::clamp(color.r, 0.0f, 1.0f))
	, g(glm::clamp(color.g, 0.0f, 1.0f))
	, b(glm::clamp(color.b, 0.0f, 1.0f))
	, a(1.0f)
{}

v3d::Color4F::Color4F(const glm::vec4 & color)
	: r(glm::clamp(color.r, 0.0f, 1.0f))
	, g(glm::clamp(color.g, 0.0f, 1.0f))
	, b(glm::clamp(color.b, 0.0f, 1.0f))
	, a(glm::clamp(color.a, 0.0f, 1.0f))
{}

bool v3d::Color4F::operator==(const Color3B& right) const
{
	return (*this == Color4F(right));
}

bool v3d::Color4F::operator==(const Color3F& right) const
{
	return (r == right.r && g == right.g && b == right.b && a == 1.0f);
}

bool v3d::Color4F::operator==(const Color4B& right) const
{
	return (*this == Color4F(right));
}

bool v3d::Color4F::operator==(const Color4F& right) const
{
	return (r == right.r && g == right.g && b == right.b && a == right.a);
}

bool v3d::Color4F::operator==(const glm::vec3 & right) const
{
	return (r == right.r && g == right.g && b == right.b && a == 1.0f);
}

bool v3d::Color4F::operator==(const glm::vec4 & right) const
{
	return (r == right.r && g == right.g && b == right.b && a == right.a);
}

bool v3d::Color4F::operator!=(const Color3B& right) const
{
    return !(*this == right);
}

bool v3d::Color4F::operator!=(const Color3F& right) const
{
    return !(*this == right);
}

bool v3d::Color4F::operator!=(const Color4B& right) const
{
    return !(*this == right);
}

bool v3d::Color4F::operator!=(const Color4F& right) const
{
    return !(*this == right);
}

bool v3d::Color4F::operator!=(const glm::vec3 & right) const
{
	return !(*this == right);
}

bool v3d::Color4F::operator!=(const glm::vec4 & right) const
{
	return !(*this == right);
}

v3d::Color4F v3d::Color4F::operator+(const Color4F & right) const
{
	return Color4F(r + right.r, g + right.g, b + right.b, a + right.a);
}

v3d::Color4F v3d::Color4F::operator-(const Color4F & right) const
{
	return Color4F(r - right.r, g - right.g, b - right.b, a - right.a);
}

v3d::Color4F v3d::Color4F::operator*(const Color4F & right) const
{
	return Color4F(r * right.r, g * right.g, b * right.b, a * right.a);
}

v3d::Color4F v3d::Color4F::operator*(const float right) const
{
	return Color4F(r * right, g * right, b * right, a * right);
}

v3d::Color4F v3d::Color4F::operator/(const float right) const
{
	return Color4F(r / right, g / right, b / right, a / right);
}

void v3d::Color4F::setR(const float r)
{
	this->r = r;
}

void v3d::Color4F::setG(const float g)
{
	this->g = g;
}

void v3d::Color4F::setB(const float b)
{
	this->b = b;
}

void v3d::Color4F::setAlpha(const float a)
{
	this->a = a;
}

void v3d::Color4F::clamp()
{
	r = glm::clamp(r, 0.0f, 1.0f);
	g = glm::clamp(g, 0.0f, 1.0f);
	b = glm::clamp(b, 0.0f, 1.0f);
	a = glm::clamp(a, 0.0f, 1.0f);
}

glm::vec4 v3d::Color4F::toVec4() const
{
	return glm::vec4(r, g, b, a);
}
