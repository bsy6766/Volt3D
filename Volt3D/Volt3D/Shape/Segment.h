/**
*	@file Segment.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_SEGMENT_H
#define V3D_SEGMENT_H

#include "Utils/Macros.h"

#include <glm/glm.hpp>

namespace v3d
{
	/**
	*	@class Segment
	*	@brief Defines a line segment with end points in 3D space.
	*
	*	@group shape
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Segment
	{
	public:
		// Constructor
		Segment();

		/**
		*	Constructor with first and second point of segment.
		*	@param p1 First point of segment.
		*	@param p2 Second point of segment.
		*/
		Segment(const glm::vec3& p1, const glm::vec3& p2);

		// Destructor
		~Segment();

		glm::vec3 p1;
		glm::vec3 p2;

		/**
		*	Get length of segment.
		*	@return Length of segment.
		*/
		float getLength() const;
				
		/** Log Segment. */
		void print() const;
	};
}

#endif