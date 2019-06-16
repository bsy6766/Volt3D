/**
*	@file AABB.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3DAABB_H
#define V3DAABB_H

#include <glm/glm.hpp>

#include <array>

#include "Utils/Macros.h"

namespace v3d
{
	/**
	*	@class AABB
	*	@brief Axis Aligned Bounding box
	*
	*	AABB is represented by position and the half extent (radius) in all 3 axes.
	*
	*	@group shape
	*	
	*	@since 1.0
	*/
	class VOLT3D_DLL AABB
	{
	public:
		/**
		*	Constructor.
		*	Creates 0 sized AABB on the origin.
		*/
		AABB();

		/**
		*	Constructor.
		*	Creates AABB with parameters.
		*	@param position A position of the AABB. This is the center position of AABB.
		*	@param halfExtents A half extent of the AABB. 
		*/
		AABB(const glm::vec3& position, const glm::vec3& halfExtents);
		
		// destructor
		~AABB();
		
		glm::vec3 position;
		glm::vec3 halfExtents;

		/**
		*	Get minimum point of AABB.
		*	@return Minimum point of AABB.
		*/
		glm::vec3 getMin() const;

		/**
		*	Get maximum point of AABB.
		*	@return Maximum point of AABB.
		*/
		glm::vec3 getMax() const;
		
		/**
		*	Get points of AABB.
		*	The order of list is Bottom left, Top Left, Top Right, Bottom Right, and front to back in z axis
		*	@return The list of all 8 points of AABB.
		*/
		std::array<glm::vec3, 8> getPoints() const;

		/** Log AABB */
		void print() const;
	};
}

#endif