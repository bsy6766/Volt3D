/**
*	@file Triangle2D.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_TRIANGLE_2D_H
#define V3D_TRIANGLE_2D_H

#include "Utils/Macros.h"

#include <glm/glm.hpp>

namespace v3d
{
	/**
	*	@class Triangle2D
	*	@brief 2D Triangle.
	*
	*	@group shape
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Triangle2D
	{
	public:
		// Constructor
		Triangle2D();

		/**
		*	Constructor with 3 points.
		*	@param [in] p1 First point of triangle
		*	@param [in] p2 Second point of triangle
		*	@param [in] p3 Third point of triangle
		*/
		Triangle2D(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);

		// Destructor
		~Triangle2D();

		glm::vec2 p1;
		glm::vec2 p2;
		glm::vec2 p3;

		/** Log Triangle2D. */
		void print() const;
	};
}

#endif