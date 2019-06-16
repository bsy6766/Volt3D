/**
*	@file Plane.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_PLANE_H
#define V3D_PLANE_H

#include <glm/glm.hpp>

#include "Utils/Macros.h"

namespace v3d
{
	/**
	*	@class Plane
	*	@brief An infinitely extending plane in 3D.
	*	
	*	Plane is a flat surface that extends indefinitely in all directions. 
	*	Plane requires 3 colinear points to be defined.
	*
	*	@group shape
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Plane
	{
	public:
		// constructor
		Plane();
				
		// Constructor with value
		Plane(const glm::vec3& normal, const float distanceToOrigin);
		
		// Default destructor
		~Plane();

		glm::vec3 normal;
		float distanceToOrigin;
				
		/**
		*	Set normal vector and distance to origin of plane.
		*	@param normal A normal vector to set.
		*	@param distanceToOrigin A distance to origin to set.
		*	@note normal gets normalized.
		*/
		void set(const glm::vec3& normal, const float distanceToOrigin);
		
		/** Log Plane. */
		void print() const;
	};
}

#endif