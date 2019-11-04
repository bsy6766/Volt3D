/**
*	@file Color4F.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_COLOR_4B
#define V3D_COLOR_4B

#include <glm/glm.hpp>

#include "Utils/Macros.h"

V3D_NS_BEGIN

// Foward declaration
class Color3B;
class Color3F;
class Color4F;

/**
*	@class Color4B
*	@brief RGBA color in unsigned char.
*
*	@since 1.0
*/
class VOLT3D_DLL Color4B
{
public:
	// Common color constants
	static const Color4B WHITE;
	static const Color4B RED;
	static const Color4B ORANGE;
	static const Color4B YELLOW;
	static const Color4B GREEN;
	static const Color4B BLUE;
	static const Color4B MAGENTA;
	static const Color4B GRAY;
	static const Color4B BLACK;

public:
	// Constructor
	Color4B();
	Color4B(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
	Color4B(const Color3B& other, const unsigned char a = 255);
	Color4B(const Color3F& other, const unsigned char a = 255);
	Color4B(const Color4F& other);
	Color4B( const glm::uvec3& color, const unsigned char a = 255 );
	Color4B( const glm::uvec4& color );

	// Equal operators 
	bool operator==(const Color3B& right) const;
	bool operator==(const Color3F& right) const;
	bool operator==(const Color4B& right) const;
	bool operator==(const Color4F& right) const;

	// Not equal operatorss
	bool operator!=(const Color3B& right) const;
	bool operator!=(const Color3F& right) const;
	bool operator!=(const Color4B& right) const;
	bool operator!=(const Color4F& right) const;

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	// Set R value [0, 255]
	void setR(const unsigned char r);

	// Set G value [0, 255]
	void setG(const unsigned char g);

	// Set B value [0, 255]
	void setB(const unsigned char b);

	// Set Alpha value [0, 255]
	void setAlpha(const unsigned char a);

	// Convert to glm
	glm::uvec4 toVec4() const;
};

V3D_NS_END

#endif