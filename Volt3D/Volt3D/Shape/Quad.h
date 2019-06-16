/**
*	@file Quad.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_QUAD_H
#define V3D_QUAD_H

#include <glm/glm.hpp>

#include "Utils/Macros.h"

namespace v3d
{
	/**
	*	@class Quad
	*	@biref A 3D quadrilateral.
	*
	*	@group shape
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Quad
	{
	public:
		// Constructor
		Quad();

		/**
		*	Constructor
		*	@param p1 First point of quad. (min x, min y)
		*	@param p2 Second point of quad. (min x, max y)
		*	@param p3 Third point of quad. (max x, min y)
		*	@param p4 Fourth point of quad. (max x, max y)
		*/
		Quad(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4);

		// Destructor
		~Quad();

		glm::vec3 p1;
		glm::vec3 p2;
		glm::vec3 p3;
		glm::vec3 p4;

		/** Log quad information. */
		void print() const;
	};
}

#endif