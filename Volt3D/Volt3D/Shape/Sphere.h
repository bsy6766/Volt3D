/**
*	@file Sphere.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_SPHERE_H
#define V3D_SPHERE_H

#include <glm/glm.hpp>

#include "Utils/Macros.h"

namespace v3d
{
	/**
	*	@class Sphere
	*	@brief 3D Sphere with position and radius
	*
	*	@group shape
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Sphere
	{
	public:
		// Constructor
		Sphere();

		/**
		*	Constructor
		*	@param [in] radius Radius of sphere
		*	@param [in] position Position of sphere
		*/
		Sphere(const float radius, const glm::vec3& position);

		// Destructor
		~Sphere();

		float radius;
		glm::vec3 position;

		/**
		*	Get diameter of Sphere.
		*	@return Diameter of Sphere.
		*/
		float getDiameter() const;

		/** Log Sphere. */
		void print() const;
	};
}

#endif