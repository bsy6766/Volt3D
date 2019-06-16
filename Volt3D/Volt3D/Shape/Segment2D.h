/**
*	@file Segment2D.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_SEGMENT_2D_H
#define V3D_SEGMENT_2D_H

#include "Utils/Macros.h"

#include <glm/glm.hpp>

namespace v3d
{
	/**
	*	@class Segment
	*	@brief Defines a line segment with end points in 2D space.
	*
	*	@group shape
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Segment2D
	{
	public:
		// Constructor
		Segment2D();

		/**
		*	Constructor with first and second point of segment.
		*	@param p1 First point of segment.
		*	@param p2 Second point of segment.
		*/
		Segment2D(const glm::vec2& p1, const glm::vec2& p2);

		// Destructor
		~Segment2D();

		glm::vec2 p1;
		glm::vec2 p2;

		/**
		*	Get length of segment.
		*	@return Length of segment.
		*/
		float getLength() const;

		/**
		*	Get closest point from the given point and segment.
		*	@param point A point to get clostest point from segment.
		*	@return A closest point.
		*/
		glm::vec2 getClosestPoint(const glm::vec2& point, float& t);

		/** Log Segment2D. */
		void print() const;
	};
}

#endif