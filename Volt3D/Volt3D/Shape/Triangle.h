/**
*	@file Triangle.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_TRIANGLE_H
#define V3D_TRIANGLE_H

#include <glm/glm.hpp>

#include "Utils/Macros.h"

namespace v3d
{

	/**
	*	@class Triangle
	*	@brief 3D Triangle.
	*
	*	@group shape
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Triangle
	{
	public:
		// Constructor
		Triangle();

		/**
		*	Constructor
		*	@param [in] p1 First point of triangle
		*	@param [in] p2 Second point of triangle
		*	@param [in] p3 Third point of triangle
		*/
		Triangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

		/**
		*	Constructor	
		*	@param [in] p1 First point of triangle
		*	@param [in] p2 Second point of triangle
		*	@param [in] p3 Third point of triangle
		*	@param [in] n Normal of triangle surface. Used for raycasting.
		*/
		Triangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& n);

		// destructor.
		~Triangle();

		glm::vec3 p1;
		glm::vec3 p2;
		glm::vec3 p3;
		glm::vec3 n;

		// Checks orientation of triangle. If triangle is clockwise, reverse to counter clockwise
		//void checkOrientation();

		/** Log Triangle. */
		void print() const;
	};
}

#endif