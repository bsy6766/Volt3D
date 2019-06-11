/**
*	@file Color3B.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_COLOR_3B
#define V3D_COLOR_3B

#include <glm/glm.hpp>

#include "Utils/Macros.h"

namespace v3d
{
	// Foward declaration
	class Color3F;
	class Color4B;
	class Color4F;

	/**
	*	@class Color3B
	*	@brief RGB color in unsigned char
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Color3B
	{
	public:
		// Constructor
		Color3B();
		Color3B(const unsigned char r, const unsigned char g, const unsigned char b);
		Color3B(const Color3F& other);
		Color3B(const Color4B& other);
		Color3B(const Color4F& other);

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

		// Set R value [0, 255]
		void setR(const unsigned char r);

		// Set G value [0, 255]
		void setG(const unsigned char g);

		// Set B value [0, 255]
		void setB(const unsigned char b);

		// Convert to glm
		glm::ivec3 toIVec3() const;

		// Common color constants
		static const Color3B WHITE;
	    static const Color3B RED;
	    static const Color3B ORANGE;
	    static const Color3B YELLOW;
	    static const Color3B GREEN;
	    static const Color3B BLUE;
	    static const Color3B MAGENTA;
	    static const Color3B GRAY;
	    static const Color3B BLACK;
	};
}

#endif