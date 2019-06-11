/**
*	@file Color3F.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_COLOR_3F
#define V3D_COLOR_3F

#include <glm/glm.hpp>

#include "Utils/Macros.h"

namespace v3d
{
	// Foward declaration
	class Color3B;
	class Color4B;
	class Color4F;

	/**
	*	@class Color3F
	*	@brief RGB color in float
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Color3F
	{
	public:
		// Constructor
		Color3F();
		Color3F(const float r, const float g, const float b);
		Color3F(const Color3B& other);
		Color3F(const Color4B& other);
		Color3F(const Color4F& other);
		Color3F(const glm::vec3& color);

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

		// Arithmetic operators
		Color3F operator+(const Color3F& right) const;
		Color3F operator-(const Color3F& right) const;
		Color3F operator*(const float right) const;
		Color3F operator/(const float right) const;

		float r;
		float g;
		float b;

		// Set RGB [0.0f, 1.0f]
		void set(const glm::vec3& color);

		// Set R value [0.0f, 1.0f]
		void setR(const float r);

		// Set G value [0.0f, 1.0f]
		void setG(const float g);

		// Set B value [0.0f, 1.0f]
		void setB(const float b);

		// Convert to glm
		glm::vec3 toVec3() const;

		// Common colors
		static const Color3F WHITE;
	    static const Color3F RED;
	    static const Color3F ORANGE;
	    static const Color3F YELLOW;
	    static const Color3F GREEN;
	    static const Color3F BLUE;
	    static const Color3F MAGENTA;
	    static const Color3F GRAY;
	    static const Color3F BLACK;
	};
}

#endif