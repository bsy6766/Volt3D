/**
*	@file OrientedRect.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_ORIENTED_RECT_H
#define V3D_ORIENTED_RECT_H

#include <glm/glm.hpp>

#include "Utils/Macros.h"

namespace v3d
{
	/**
	*	@class OrientedRect
	*	@brief A rectangle that can be rotated in 2D space.
	*
	*	@group shape
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL OrientedRect
	{
	public:
		/**
		*	Constructor.
		*	Creates 0 sized OrientedRect.
		*/
		OrientedRect();

		/**
		*	Constructor.
		*	Creates OrientedRect with position and size.
		*	@param position A position of OrientedRect.
		*	@param halfExtents A half extent of OrientedRect.
		*	@param rotation A rotation angle in degree.
		*/
		OrientedRect(const glm::vec2& position, const glm::vec2& halfExtents, const float rotation);

		// Destructor
		~OrientedRect();

		glm::vec2 position;
		glm::vec2 halfExtents;
		float rotation;

		/**
		*	Get 4 corner points of OrientedRect.
		*	The order of return value is Left Bottom, Left Top, Right Top, and Right Bottom.
		*	@return List of 4 corner poitns of OrientedRect.
		*/
		std::array<glm::vec2, 4> getPoints() const;

		/** Log OrientedRect */
		void print() const;
	};
}

#endif