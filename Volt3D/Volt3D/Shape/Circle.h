/**
*	@file Circle.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_CIRCLE_H
#define V3D_CIRCLE_H

#include "Utils/Macros.h"

#include <glm/glm.hpp>

namespace v3d
{
	/**
	*	@class Circle
	*	@brief A circle defiend in 2d position and radius (in pixel).
	*
	*	@group shape
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Circle
	{
	public:
		/**
		*	Constructor.
		*	Creates circle with 0 radius on origin.
		*/
		Circle();

		/**
		*	Constructor.
		*	Creates Circle with parameters.
		*	@param position A position of the AABB. This is the center position of AABB.
		*	@param radius A radius of this circle.
		*/
		Circle(const glm::vec2& position, const float radius);

		// Destructor
		~Circle();
		
		glm::vec2 position;
		float radius;

		/**
		*	Get diameter of this circle.
		*	@return radius * 2;
		*/
		inline float getDiameter() const;

		/** Log Circle */
		void print() const;
	};
}

#endif