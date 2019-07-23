#include <PreCompiled.h>

#include "DebugCamera.h"

#include "Core/Application.h"
#include "Core/Engine.h"
#include "Input/InputManager.h"
#include "Input/KeyCode.h"

v3d::DebugCamera::DebugCamera(std::string_view name)
	: v3d::Camera(name)
	, translationSpeed(0.0f)
	, rotationSpeed(0.0f)
	, input(v3d::Application::getInstance().getEngine()->getInputManager())
{}

v3d::DebugCamera::~DebugCamera() {}

v3d::DebugCamera* v3d::DebugCamera::create( std::string_view name, const v3d::ProjectionType projectionType, const float fovy, const float nears, const float fars, const float screenWidth, const float screenHeight, const float transitionSpeed, const float rotationSpeed )
{
	v3d::DebugCamera* newDebugCamera = new (std::nothrow) v3d::DebugCamera( name );

	if (newDebugCamera)
	{
		if (newDebugCamera->init( projectionType, fovy, nears, fars, screenWidth, screenHeight ))
		{
			newDebugCamera->translationSpeed = transitionSpeed;
			newDebugCamera->rotationSpeed = rotationSpeed;
			return newDebugCamera;
		}
		delete newDebugCamera;
	}

	return nullptr;
}

void v3d::DebugCamera::onUpdate( const float delta )
{
	if (active)
	{
		// check W and S
		if (input.isKeyPressed( v3d::KeyCode::eW, false ))
		{
			// move foward
			addPosition( getMovedDist( -180.0f, delta * translationSpeed ) );
		}
		else if (input.isKeyPressed( v3d::KeyCode::eS, false ))
		{
			// move backward
			addPosition( getMovedDist( 0.0f, delta * translationSpeed ) );
		}

		// check A and D
		if (input.isKeyPressed( v3d::KeyCode::eA, false ))
		{
			// move left
			addPosition( getMovedDist( 90.0f, delta * translationSpeed ) );

		}
		else if (input.isKeyPressed( v3d::KeyCode::eD, false ))
		{
			// move right
			addPosition( getMovedDist( -90.0f, delta * translationSpeed ) );
		}

		// check space and left shift
		if (input.isKeyPressed( v3d::KeyCode::eSpace, false ))
		{
			// move left
			addPositionY( delta * translationSpeed );

		}
		else if (input.isKeyPressed( v3d::KeyCode::eLeftShift, false ))
		{
			// move down
			addPositionY( delta * translationSpeed * -1.0f );
		}

		// reset translation
		if (input.isKeyPressed( v3d::KeyCode::eX, true ))
		{
			setPosition( glm::vec3( 0.0f, 0.0f, 0.0f ) );
		}

		// reset rotation
		if (input.isKeyPressed( v3d::KeyCode::eZ, true ))
		{
			setRotation( glm::vec3( 0.0f, 0.0f, 0.0f ) );
		}

		// mouse
		if (input.didMouseMove())
		{
			// get distance that mouse moved with delta and speed applied
			const glm::vec2 mouseDelta = input.getMouseMovedDistance() * delta * rotationSpeed;

			// rotate
			addRotation( glm::vec2( -mouseDelta.y, mouseDelta.x ) );
		}

		v3d::Camera::onUpdate( delta );
	}
}

glm::vec3 v3d::DebugCamera::getMovedDist(const float angleMod, const float distance)
{
	// get current y rotation of camera
	float curRotY = getRotation().y;

	// add angle mode
	curRotY += angleMod;

	const glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(curRotY), glm::vec3(0.0f, 1.0f, 0.0f));
	const auto movedDist = glm::inverse(rotateMat) * glm::vec4(0, 0, distance, 1);

	return glm::vec3(movedDist);
}

void v3d::DebugCamera::toggle()
{
	active = !active;
}

float v3d::DebugCamera::getTranslationSpeed() const
{
	return translationSpeed;
}

void v3d::DebugCamera::setTranslationSpeed( const float newTranslationSpeed )
{
	translationSpeed = newTranslationSpeed;
}

float v3d::DebugCamera::getRotationSpeed() const
{
	return rotationSpeed;
}

void v3d::DebugCamera::setRotationSpeed( const float newRotationSpeed )
{
	rotationSpeed = newRotationSpeed;
}