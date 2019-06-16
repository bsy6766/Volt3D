/**
*	@file Rect.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_RECT_H
#define V3D_RECT_H

#include <glm/glm.hpp>

#include <array>

#include "Utils/Macros.h"

namespace v3d
{
	class AABB;

	/**
	*	@class Rect
	*	@brief 2D rectangle defined with min and max position.
	*
	*	@group shape
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Rect
	{
	public:
		// Constructor
		Rect();

		/**
		*	Constructor with minimum and maximum position of Rect.
		*	@param min A minimum position of Rect.
		*	@param max A maximum poistion of Rect.
		*/
		Rect(const glm::vec2& min, const glm::vec2& max);

		/**
		*	Constructor with AABB.
		*	@param aabb An AABB to construct Rect.
		*	@note z value in aabb will be ignored. Only x and y value is used.
		*/
		Rect(const v3d::AABB& aabb);
		
		// Destructor
		~Rect();

		glm::vec2 min;
		glm::vec2 max;

		/**
		*	Set Rect with center and the size.
		*	@param center The cetner position of Rect.
		*	@param size The size of Rect.
		*/
		void set(const glm::vec2& center, const glm::vec2& size);

		/**
		*	Get size of Rect.
		*	@return The size of Rect.
		*/
		glm::vec2 getSize() const;

		/**
		*	Get center position of Rect.
		*	@return The cetner position of Rect.
		*/
		glm::vec2 getCenter() const;

		/**
		*	Get four points of Rect.
		*	The order of return value is Left Bottom, Left Top, Right Top, Right Bottom.
		*/
		std::array<glm::vec2, 4> getPoints() const;

		/*
		void merge(const v3d::Rect& other);

		bool containsPoint(const glm::vec2& point) const;

		bool doesIntersectsWith(const Rect& other) const;

		bool doesIntersectsWith(const v3d::Quad2D& ohter) const;

		Rect getIntersectingRect(const Rect& other) const;
		*/
		
		/** Log Rect. */
		void print() const;
	};
}

#endif