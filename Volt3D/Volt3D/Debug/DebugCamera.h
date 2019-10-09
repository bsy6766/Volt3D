/**
*	@file BuildConfig.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include "Config/DebugConfig.h"

#ifndef VOLT3D_DEBUG_CAMERA_H
#define VOLT3D_DEBUG_CAMERA_H

#include <string_view>

#include "Engine/Camera.h"

#include "Utils/Macros.h"

namespace v3d
{
	// Foward declaration
	class InputManager;

	/**
	*	@class DebugCamera
	*	@brief Derives Camera class with extra input features.
	*
	*	Debug camera can move around using the W,A,S,D,Left shift, 
	*	and space key and look around using the mouse.
	*
	*	@group debug
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL DebugCamera : public v3d::Camera
	{
		friend class Director;

	protected:
		// constructor
		DebugCamera(std::string_view name);
		
		// speeds
		float translationSpeed;
		float rotationSpeed;
		
		/**
		*	Update node.
		*	This updates everything needs for this node.
		*	@param delta Elapsed time for current frame
		*/
		void onUpdate( const float delta ) override;

		// input manager ref
		v3d::InputManager& input;

		// Get camera moving distance
		glm::vec3 getMovedDist(const float angleMod, const float distance);

	public:
		// destructor
		~DebugCamera();
		
		/**
		*	Create debug camera
		*	@param name Name of camera
		*	@param projectionType A projection type of camera. (perspective or orthographic)
		*	@param fovy Vertical field of view angle [0.0f, 360.0f]
		*	@param nears Near clipping plane [0.0001f, float max]
		*	@param fars Far clipping plane [nears, float max]
		*	@param screenWidth Screen width
		*	@param screenHeight Screen height
		*	@return A new camera instance if success. Else, nullptr.
		*/
		static v3d::DebugCamera* create( std::string_view name, const v3d::ProjectionType projectionType, const float fovy, const float nears, const float fars, const float screenWidth, const float screenHeight, const float transitionSpeed = 200.0f, const float rotationSpeed = 20.0f );

		// Toggle
		void toggle();

		// Get current translation speed
		float getTranslationSpeed() const;

		/**
		*	Set translation speed.
		*	@param newTranslationSpeed A new translation speed to set.
		*/
		void setTranslationSpeed(const float newTranslationSpeed);

		// Get current rotation speed
		float getRotationSpeed() const;

		/**
		*	Set rotation speed.
		*	@param newRotationSpeed A new rotation speed to set.
		*/
		void setRotationSpeed(const float newRotationSpeed);

	};
}

#endif