/**
*	@file OBB.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_OBB_H
#define V3D_OBB_H

#include <glm/glm.hpp>

#include "Utils/Macros.h"

namespace v3d
{
	/**
	*	@class OBB
	*	@brief An oriented boundig box.
	*/
	class VOLT3D_DLL OBB
	{
	public:
		/**
		*	Constructor.
		*	Creates 0 sized OBB on origin with no orientation.
		*/
		OBB();

		/**
		*	Constructor.
		*	Creates OBB with position and size.
		*	@param position A position of OBB.
		*	@param halfExtents A half extent of OBB.
		*/
		OBB(const glm::vec3& position, const glm::vec3& halfExtents);

		/**
		*	Constructor.
		*	Creates OBB with position, size, and orientation.
		*	@param position A position of OBB.
		*	@param halfExtents A half extent of OBB.
		*	@param orientation A orientation matrix of OBB.
		*/
		OBB(const glm::vec3& position, const glm::vec3& halfExtents, const glm::mat3& orientation);

		// Destructor
		~OBB();

		glm::vec3 position;
		glm::vec3 halfExtents;
		glm::mat3 orientation;

		/** Log OBB */
		void print() const;
	};
}

#endif