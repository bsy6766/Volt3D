/**
*	@file Camera.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef VOLT3D_CAMERA_H
#define VOLT3D_CAMERA_H

#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "Node/TransformNode.h"
#include "ProjectionType.h"
#include "Node/NodeType.h"
#include "Shape/Rect.h"
#include "Shape/OrientedRect.h"
#include "Utils/Macros.h"

namespace v3d
{
	class Frustum;
	//class Segment;

	/**
	*	@class Camera
	*	
	*	@todo: document this
	*/
	class VOLT3D_DLL Camera : public v3d::TransformNode
	{
	private:
		// Projection Type
		v3d::ProjectionType projectionType;

	private:
		// Constructor with name
		Camera(const std::string& name);
		
		// =================== Attribute ====================
		float fovy;					// Field of view Y
		float fovx;					// Field of view X
		float aspect;				// Screen ratio (w/h)
		float nears;				// Near clipping plane
		float fars;					// Far clipping plane
		glm::vec2 screenSize;
		// ==================================================


		// ===================== matrix =====================
		glm::mat4 projectionMat;				// Cache projection matrix
		glm::mat4 viewMat;
		glm::mat4 screenSpaceMatrix;			// Distance of z from origin to screen space.
		// ==================================================

		// =============== Frustrums (sort of) ==============
		std::shared_ptr<v3d::Frustum> frustum;			// 3D
		/*
		v3d::OrientedRect screenRect2D;					// 2D space is defined with camera's xy position, z rotation and screen size.
		v3d::Rect screenRectUI;							// UI space is defined with origin xy position and screen size.
		// ==================================================
		*/

		// ======================= Init =====================
		bool init(const v3d::ProjectionType projectionType, const float fovy, const float nears, const float fars, const float screenWidth, const float screenHeight);
		bool initFrustum();
		// ==================================================


		// ===================== Update =====================
		void updateFovx();
		void updateFrustrum(const glm::mat4& MV);
		void updateScreenSpaceMatrix();
		virtual void updateModelMatrix() override;
		// ==================================================

	public:
		// Destructor
		~Camera();

		/**
		*	Create camera
		*	@param name Name of camera
		*	@param projectionType A projection type of camera. (perspective or orthographic)
		*	@param fovy Vertical field of view angle [0.0f, 360.0f]
		*	@param nears Near clipping plane [0.0001f, float max]
		*	@param fars Far clipping plane [nears, float max]
		*	@param screenWidth Screen width
		*	@param screenHeight Screen height
		*	@return A new camera instance if success. Else, nullptr.
		*/
		static v3d::Camera* create(const std::string& name, const v3d::ProjectionType projectionType, const float fovy, const float nears, const float fars, const float screenWidth, const float screenHeight);
				
		// Get projection matrix (Perspective or orthographic)
		glm::mat4 getProjectionMatrix() const;
		
		// Get view matrix
		glm::mat4 getViewMatrix(const v3d::NodeType targetNodeType) const;

		// Get projection matrix * view matrix
		glm::mat4 getProjectionViewMatrix(const v3d::NodeType targetNodeType) const;

		// Get orientation amtrix
		glm::mat4 getOrientation() const;

		/**
		*	Set camera to perspective projection.
		*	This also updates frustum.
		*	@return true if success. If camera is already perspective, returns false.
		*/
		bool setToPerspective();

		/**
		*	Set camera to orthographic projection.
		*	@return true if success.  If camera is already orthographic, returns false.
		*/
		bool setToOrthographic();

		// Get projection type
		v3d::ProjectionType getProjectionType() const;

		// Update projection matrix.
		void updateProjectionMatrix();

		// Update view matrix
		void updateViewMatrix();

		// Set vertical fov.
		void setFovy(const float fovy);

		// Get fovy
		float getFovy() const;

		// Get fovx
		float getFovx() const;

		// Set near
		void setNear(const float nears);

		// Get near
		float getNear() const;

		// Set far
		void setFar(const float fars);

		// get far
		float getFar() const;

		// project point to screen space
		glm::vec2 projectPoint2D(const glm::vec3& worldPoint) const;

		// Get frustum instance
		//std::shared_ptr<v3d::Frustum> getFrustum() const;

		// Get ray segment from camera's view
		//v3d::Segment getRaySegment(const glm::vec2& mousePosition) const;

		// Update screen size and aspect. Default camera updates screen size automatically.
		void updateScreenSizeAndAspect(const float screenWidth, const float screenHeight);
		
		// Check if node is visible by projection
		bool isNodeVisible(v3d::TransformNode* node, const v3d::ProjectionType projectionType) const;

		// Check if node is visible by projection
		bool isNodeVisible(const v3d::TransformNode& node) const;

		/**
		*	Update node.
		*	This updates everything needs for this node.
		*	@param delta Elapsed time for current frame
		*/
		void onUpdate(const float delta) override;

		/**
		*	Get default node type.
		*	Camera's default node type is 3D.
		*	@return v3d::NodeType::_3D
		*/
		v3d::NodeType getDefaultNodeType() const override;
		
		// Log camera information. For debug.
		void print(const bool printBase) const override;
	};
}

#endif