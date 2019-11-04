/**
*	@file Color4F.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_COLOR_4F
#define V3D_COLOR_4F

#include <glm/glm.hpp>

#include "Utils/Macros.h"

V3D_NS_BEGIN

// Foward declaration
class Color3B;
class Color3F;
class Color4B;

/**
*	@class Color4F
*	@brief RGBA color in float
*
*	@since 1.0
*/
class VOLT3D_DLL Color4F
{
public:
	// Common color constants
	static const Color4F WHITE;
	static const Color4F RED;
	static const Color4F ORANGE;
	static const Color4F YELLOW;
	static const Color4F GREEN;
	static const Color4F BLUE;
	static const Color4F MAGENTA;
	static const Color4F GRAY;
	static const Color4F BLACK;

public:
	// Constructor
	Color4F();
	Color4F(const float r, const float g, const float b, const float a);
	Color4F(const Color3B& other, const float a = 1.0f);
	Color4F(const Color3F& other, const float a = 1.0f);
	Color4F(const Color4B& other);
	Color4F(const glm::vec3& color, const float a = 1.0f);
	Color4F(const glm::vec4& color);

	// Equal operators 
	bool operator==(const Color3B& right) const;
	bool operator==(const Color3F& right) const;
	bool operator==(const Color4B& right) const;
	bool operator==(const Color4F& right) const;
	bool operator==(const glm::vec3& right) const;
	bool operator==(const glm::vec4& right) const;

	// Not equal operatorss
	bool operator!=(const Color3B& right) const;
	bool operator!=(const Color3F& right) const;
	bool operator!=(const Color4B& right) const;
	bool operator!=(const Color4F& right) const;
	bool operator!=(const glm::vec3& right) const;
	bool operator!=(const glm::vec4& right) const;

	// Arithmetic operators
	Color4F operator+(const Color4F& right) const;
	Color4F operator-(const Color4F& right) const;
	Color4F operator*(const Color4F& right) const;
	Color4F operator*(const float right) const;
	Color4F operator/(const float right) const;

	// Color value
	float r;
	float g;
	float b;
	float a;

	// Set R value [0.0f, 1.0f]
	void setR(const float r);

	// Set G value [0.0f, 1.0f]
	void setG(const float g);

	// Set B value [0.0f, 1.0f]
	void setB(const float b);

	// Set Alpha value [0.0f, 1.0f]
	void setAlpha(const float a);

	// Clamp color values to [0.0f, 1.0f]
	void clamp();

	// Convert to glm
	glm::vec4 toVec4() const;
};

V3D_NS_END

#endif