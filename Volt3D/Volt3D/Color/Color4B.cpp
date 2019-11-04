/**
*	@file Color4F.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Color4B.h"

#include "Color3B.h"
#include "Color3F.h"
#include "Color4F.h"

V3D_NS_BEGIN

const v3d::Color4B v3d::Color4B::WHITE  (255, 255, 255, 255);
const v3d::Color4B v3d::Color4B::RED    (255,   0,   0, 255);
const v3d::Color4B v3d::Color4B::ORANGE (255, 127,   0, 255);
const v3d::Color4B v3d::Color4B::YELLOW (255, 255,   0, 255);
const v3d::Color4B v3d::Color4B::GREEN  (  0, 255,   0, 255);
const v3d::Color4B v3d::Color4B::BLUE   (  0,   0, 255, 255);
const v3d::Color4B v3d::Color4B::MAGENTA(255,   0, 255, 255);
const v3d::Color4B v3d::Color4B::GRAY   (166, 166, 166, 255);
const v3d::Color4B v3d::Color4B::BLACK  (  0,   0,   0, 255);

Color4B::Color4B()
	: r(0)
	, g(0)
	, b(0)
	, a(0)
{}

Color4B::Color4B(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
	: r(r)
	, g(g)
	, b(b)
	, a(a)
{}

Color4B::Color4B(const Color3B& other, const unsigned char a)
	: r(other.r)
	, g(other.g)
	, b(other.b)
	, a(a)
{}

Color4B::Color4B(const Color3F & other, const unsigned char a)
	: r(static_cast<unsigned char>(other.r * 255.0f))
	, g(static_cast<unsigned char>(other.g * 255.0f))
	, b(static_cast<unsigned char>(other.b * 255.0f))
	, a(a)
{}

Color4B::Color4B(const Color4F& other)
	: r(static_cast<unsigned char>(other.r * 255.0f))
	, g(static_cast<unsigned char>(other.g * 255.0f))
	, b(static_cast<unsigned char>(other.b * 255.0f))
	, a(static_cast<unsigned char>(other.a * 255.0f))
{}

Color4B::Color4B( const glm::uvec3 & color, const unsigned char a )
	: r(color.r)
	, g(color.g)
	, b(color.b)
	, a(a)
{}

Color4B::Color4B( const glm::uvec4& color )
{
}

bool Color4B::operator==(const v3d::Color3B& right) const
{
	return (r == right.r && g == right.g && b == right.b && a == 255);
}

bool Color4B::operator==(const v3d::Color3F& right) const
{
    return (a == 255 && (*this == Color4B(right)));
}

bool Color4B::operator==(const v3d::Color4B& right) const
{
	return (r == right.r && g == right.g && b == right.b && a == right.a);
}

bool Color4B::operator==(const v3d::Color4F& right) const
{
	return (*this == Color4B(right));
}

bool Color4B::operator!=(const v3d::Color3B& right) const
{
    return !(*this == right);
}

bool Color4B::operator!=(const v3d::Color3F& right) const
{
    return !(*this == right);
}

bool Color4B::operator!=(const v3d::Color4B& right) const
{
    return !(*this == right);
}

bool Color4B::operator!=(const v3d::Color4F& right) const
{
    return !(*this == right);
}

void Color4B::setR(const unsigned char r)
{
	this->r = glm::clamp(r, static_cast<unsigned char>(0), static_cast<unsigned char>(255));
}

void Color4B::setG(const unsigned char g)
{
	this->g = glm::clamp(g, static_cast<unsigned char>(0), static_cast<unsigned char>(255));
}

void Color4B::setB(const unsigned char b)
{
	this->b = glm::clamp(b, static_cast<unsigned char>(0), static_cast<unsigned char>(255));
}

void Color4B::setAlpha(const unsigned char a)
{
	this->a = glm::clamp(a, static_cast<unsigned char>(0), static_cast<unsigned char>(255));
}

glm::uvec4 Color4B::toVec4() const
{
	return glm::uvec4(r, g, b, a);
}

V3D_NS_END