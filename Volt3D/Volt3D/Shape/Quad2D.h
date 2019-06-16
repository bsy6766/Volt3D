/**
*	@file Quad2D.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_QUAD_2D_H
#define V3D_QUAD_2D_H

#include <glm/glm.hpp>

#include "Utils/Macros.h"

namespace v3d
{
	class Rect;

	/**
	*	@class Quad2D
	*	@brief A 2D quadrilateral.
	*	@note Difference between with Rect is that Quad is not a rectangle, but a polygon.
	*
	*	@group shape
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Quad2D
	{
	public:
		// Constructor
		Quad2D();

		/**
		*	Constructor
		*	@param p1 First point of quad. (min x, min y)
		*	@param p2 Second point of quad. (min x, max y)
		*	@param p3 Third point of quad. (max x, min y)
		*	@param p4 Fourth point of quad. (max x, max y)
		*/
		Quad2D(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4);

		/**
		*	Constructor with Rect
		*	@param rect A Rect to convert to Quad2D
		*/
		Quad2D(const v3d::Rect& rect);

		// Destructor
		~Quad2D();

		glm::vec2 p1;
		glm::vec2 p2;
		glm::vec2 p3;
		glm::vec2 p4;
		
		/**
		*	Set Quad2D as Rect.
		*/
		void set(const v3d::Rect& rect);

		/*
		*	Convert Quad2D as Rect.
		*	@return Rect converted from this Quad2D.
		*/
		v3d::Rect toRect() const;

		///**
		//*	Checks if contains point.
		//*	@param point A point to check.
		//*	@return true if contains point. Else, false.
		//*/
		//bool containsPoint(const glm::vec2& point);

		//// Check if two quadrilateral intersects
		//bool doesIntersects(const v3d::Quad2D& other);

		/** Log Quad2D */
		void print();
	};
}

#endif