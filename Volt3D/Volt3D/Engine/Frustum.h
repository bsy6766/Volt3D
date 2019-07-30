/**
*	@file Frustum.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_FRUSTUM_H
#define V3D_FRUSTUM_H

#include <array>

#include "Shape/Plane.h"

#include "Utils/Macros.h"

namespace v3d
{
	class Plane;
	class AABB;
	class OBB;
//#if V3D_FRUSTRUM_DEBUG_DRAW
//	class ShapeNode;
//	class Camera;
//#endif

	/**
	*	@class Frustum
	*	@brief Contains frustum information with 6 planes of frustum
	*/
	class VOLT3D_DLL Frustum
	{
	public:
		/**
		*	@enum ClippingPlane
		*	6 frustum clipping plane
		*/
		enum ClippingPlane
		{
			NEARS = 0,
			FARS,
			LEFT,
			RIGHT,
			TOP,
			BOTTOM
		};

	private:
		// Constructor
		Frustum();

		// All 6 planes. 
		std::array<v3d::Plane, 6> clippingPlanes;
		
		//float AABBPlaneCheck(const v3d::AABB& aabb, const v3d::Plane& plane);
		//float OBBPlaneCheck(const v3d::OBB& aabb, const v3d::Plane& plane);

	public:
		// Destructor
		~Frustum();

		/**
		*	Create frustum
		*	@return v3d::Frustum instance if success. Else, nullptr.
		*/
		static v3d::Frustum* create(const glm::mat4& VPMat);
		
		/**
		*	Update frustum planes.
		*	@param VPMat View Projection matrix.
		*/
		//void updateFrustumPlanes(const glm::mat4& VPMat);

		/**
		*	Check if AABB is visible by frsutum culling
		*	@param aabb AABB to check.
		*	@return true if it's visible. Else, false.
		*/
		//bool isAABBVisible(const v3d::AABB& aabb);

		//bool isAABBVisible2(const v3d::AABB& aabb);

		/**
		*	Check if OBB is visible by frustum culling
		*	@param obb OBB to check.
		*	@return true if it's visible. Else, false.
		*/
		//bool isOBBVisible(const v3d::OBB& obb);

		/** Log frustum. */
		void print() const;

	private:
#if V3D_FRUSTRUM_DEBUG_DRAW
		v3d::ShapeNode* debugDrawShapeNode;
		void initDebugShapeNode();
		void initFrustumDebugLines(const v3d::Camera& camera);
		void renderDebugDraw();
		void releaseDebugDraw();
#endif
	};
}

#endif