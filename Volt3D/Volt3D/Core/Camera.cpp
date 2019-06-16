#include <PreCompiled.h>

#include "Camera.h"

#include "Frustum.h"

//#include "Volt3D/Core/Director.h"
#include "Utils/Logger.h"

#include "Config/BuildConfig.h"

//#include "Volt3D/Error/ErrorReport.h"
//
//#include "Volt3D/Physics/CollisionDetection.h"
//
//#include "Volt3D/Utility/String.h"

v3d::Camera::Camera(const std::string & name)
	: v3d::TransformNode(name, v3d::NodeType::e3D)
	, fovy(0)
	, fovx(0)
	, aspect(0)
	, nears(0)
	, fars(0)
	, projectionMat(1.0f)
	, viewMat(1.0f)
	, screenSpaceMatrix(1.0f)
	, frustum(nullptr)
	, screenSize(0.0f)
{}

v3d::Camera::~Camera() 
{
	//frustum.reset();
}

v3d::Camera* v3d::Camera::create(const std::string& name, const v3d::ProjectionType projectionType, const float fovy, const float nears, const float fars, const float screenWidth, const float screenHeight)
{
	// Craete new camera instance with name
	v3d::Camera* newCamera = new (std::nothrow) v3d::Camera(name);

	// Check
	if (newCamera)
	{
		// Success. Init
		if (newCamera->init(projectionType, fovy, nears, fars, screenWidth, screenHeight))
		{
			// Done.
			return newCamera;
		}

		// Delete instance
		delete newCamera;
	}
	else
	{
		// report
		//v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::CAMERA);
	}

	// Failed
	return nullptr;
}

bool v3d::Camera::init(const v3d::ProjectionType projectionType, const float fovy, const float nears, const float fars, const float screenWidth, const float screenHeight)
{
	// Can't calculate aspect if screen height is 0 or negative.
	if (screenHeight <= 0.0f) return false;
	// Screen size must not be 0 or negative
	if (screenWidth <= 0.0f) return false;

	// nears
	this->nears = nears;
	// fars
	this->fars = fars;

	// calculate aspect
	aspect = screenWidth / screenHeight;

	// save screen size
	screenSize.x = screenWidth;
	screenSize.y = screenHeight;

	// Set ui screen size
	//screenRectUI.min = -screenSize * 0.5f;
	//screenRectUI.max = screenSize * 0.5f;

	// init frustum
	if (!initFrustum()) return false;

	// set as perspective
	this->projectionType = projectionType;

	// Set fovy. this updates projection
	setFovy(fovy);

	// Update screen space matrix
	updateScreenSpaceMatrix();

	// update view matrix
	updateViewMatrix();

	// update frustum
	////frustum->updateFrustumPlanes(projectionMat * viewMat);

#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
	print(true);
#endif

	// success
	return true;
}

bool v3d::Camera::initFrustum()
{
	// create
	v3d::Frustum* newFrustum = v3d::Frustum::create(projectionMat * viewMat);

	// check
	if (newFrustum)
	{
		//s et
		frustum = std::move(std::shared_ptr<v3d::Frustum>(newFrustum));

		// done.
		return true;
	}

	// Failed.
	return false;
}

void v3d::Camera::updateFovx()
{
	// Set fovx. Refernce: http://wiki.panotools.org/Field_of_View
	fovx = glm::degrees(2.0f * atan(tan(glm::radians(fovy * 0.5f)) * aspect));
}

void v3d::Camera::updateProjectionMatrix()
{
	if (projectionType == v3d::ProjectionType::ePerspective)
	{
		projectionMat = glm::perspective(glm::radians(fovy), aspect, nears, fars);
	}
	else if (projectionType == v3d::ProjectionType::eOrthographic)
	{
		projectionMat = glm::ortho(screenSize.x * -0.5f, screenSize.x * 0.5f, screenSize.y * -0.5f, screenSize.y * 0.5f, nears, fars);
	}
}

void v3d::Camera::updateViewMatrix()
{
	//viewMat = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0));
	//viewMat = glm::rotate(viewMat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	//viewMat = glm::rotate(viewMat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	//viewMat  = viewMat * glm::translate(glm::mat4(1.0f), -position);

	// one liner
	viewMat = getOrientation() * glm::translate(glm::mat4(1.0f), -position);
}

void v3d::Camera::updateScreenSizeAndAspect(const float screenWidth, const float screenHeight)
{
	// check screen size
	if (screenHeight == 0)
	{
		// Can't divide by 0
		aspect = 0.0f;
	}
	else
	{
		// Update aspect
		aspect = screenWidth / screenHeight;
	}

	// update screen size
	screenSize = glm::vec2(screenWidth, screenHeight);

	// screen half
	glm::vec2 screenHalf = screenSize * 0.5f;

	//screenRect2D.halfExtents = screenHalf;

	//screenRectUI.min = -screenHalf;
	//screenRectUI.max = screenHalf;

	updateFovx();
	updateScreenSpaceMatrix();
	updateProjectionMatrix();

	// update frustum
	//if (frustum) frustum->updateFrustumPlanes(projectionMat * viewMat);
}

void v3d::Camera::updateFrustrum(const glm::mat4& MV)
{
	if (frustum)
	{
		//frustum->updateFrustumPlanes(MV);
	}
}

void v3d::Camera::updateScreenSpaceMatrix()
{
	screenSpaceMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -((screenSize.y * 0.5f) / tanf(glm::radians(fovy * 0.5f)))));
}

void v3d::Camera::updateModelMatrix()
{
	// Call base func
	v3d::TransformNode::updateModelMatrix();
		
	// Update 2d rect
	//screenRect2D.position = position;
	//screenRect2D.rotation = rotation.z;

	// Update view matrix
	updateViewMatrix();

	// update frustum
	//frustum->updateFrustumPlanes(projectionMat * viewMat);
}

glm::mat4 v3d::Camera::getProjectionMatrix() const
{
	return projectionMat;
}

glm::mat4 v3d::Camera::getViewMatrix(const v3d::NodeType targetNodeType) const
{
	if (targetNodeType == v3d::NodeType::e3D)
	{
		return viewMat;
	}
	else if (targetNodeType == v3d::NodeType::e2D)
	{
		return viewMat * screenSpaceMatrix;
	}
	else
	{
		return glm::mat4(1.0f);
	}
}

glm::mat4 v3d::Camera::getProjectionViewMatrix(const v3d::NodeType targetNodeType) const
{
	if (targetNodeType == v3d::NodeType::e3D)
	{
		return projectionMat * viewMat;
	}
	else if (targetNodeType == v3d::NodeType::e2D)
	{
		if (projectionType == v3d::ProjectionType::ePerspective)
		{
			return projectionMat * viewMat * screenSpaceMatrix;
		}
		else if (projectionType == v3d::ProjectionType::eOrthographic)
		{
			return projectionMat * viewMat * screenSpaceMatrix;
		}
	}
	else if (targetNodeType == v3d::NodeType::eGUI)
	{
		if (projectionType == v3d::ProjectionType::ePerspective)
		{
			return projectionMat * screenSpaceMatrix;
		}
		else if (projectionType == v3d::ProjectionType::eOrthographic)
		{
			return projectionMat;
		}
	}

	return glm::mat4(1.0f);
}

glm::mat4 v3d::Camera::getOrientation() const
{
	return glm::rotate(glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0)), glm::radians(rotation.y), glm::vec3(0, 1, 0)), glm::radians(rotation.z), glm::vec3(0, 0, 1));
}

bool v3d::Camera::setToPerspective()
{
	// check current projection type
	if (projectionType == v3d::ProjectionType::eOrthographic)
	{
		// update
		projectionType = v3d::ProjectionType::ePerspective;

		// update all matrices
		updateProjectionMatrix();
		updateViewMatrix();
		updateScreenSpaceMatrix();

		// update frustum
		//if (frustum) frustum->updateFrustumPlanes(projectionMat * viewMat);

		// Done.
		return true;
	}
	// Else, already perspective

	return false;
}

bool v3d::Camera::setToOrthographic()
{
	// check current projection type
	if (projectionType == v3d::ProjectionType::ePerspective)
	{
		// update
		projectionType = v3d::ProjectionType::eOrthographic;

		// update all matrices
		updateProjectionMatrix();
		updateViewMatrix();
		updateScreenSpaceMatrix();

		// Done.
		return true;
	}
	// Else, already orthographic

	return false;
}

v3d::ProjectionType v3d::Camera::getProjectionType() const
{
	return projectionType;
}

//std::shared_ptr<v3d::Frustum> v3d::Camera::getFrustum() const
//{
//	return frustum;
//}

//v3d::Segment v3d::Camera::getRaySegment(const glm::vec2 & mousePosition) const
//{
//	const glm::vec2 glMousePos = mousePosition + (screenSize * 0.5f);
//	const glm::vec4 viewport(0.0f, 0.0f, screenSize);
//	const glm::mat4 viewMatrix = getViewMatrix(v3d::NodeType::e2D);
//	const glm::vec3 mpNear = glm::unProject(glm::vec3(glMousePos, 0.01f), viewMatrix, projectionMat, viewport);
//	const glm::vec3 mpFar = glm::unProject(glm::vec3(glMousePos, 1.0f), viewMatrix, projectionMat, viewport);
//	
//	return v3d::Segment(mpNear, mpFar);
//}

void v3d::Camera::setFovy(const float fovy)
{
	this->fovy = fovy;
	updateFovx();

	// update projection
	updateProjectionMatrix();

	// update frustum
	//if (frustum) frustum->updateFrustumPlanes(projectionMat * viewMat);
}

float v3d::Camera::getFovy() const
{
	return fovy;
}

float v3d::Camera::getFovx() const
{
	return fovx;
}

void v3d::Camera::setNear(const float nears)
{
	this->nears = nears;

	// update projection
	updateProjectionMatrix();

	// update frustum
	//if (frustum) frustum->updateFrustumPlanes(projectionMat * viewMat);
}

float v3d::Camera::getNear() const
{
	return nears;
}

void v3d::Camera::setFar(const float fars)
{
	// set
	this->fars = fars;

	// update projection
	updateProjectionMatrix();

	// update frustum
	//if (frustum) frustum->updateFrustumPlanes(projectionMat * viewMat);
}

float v3d::Camera::getFar() const
{
	return fars;
}

glm::vec2 v3d::Camera::projectPoint2D(const glm::vec3 & worldPoint) const
{
	return glm::vec2(glm::project(worldPoint, viewMat * screenSpaceMatrix, projectionMat, glm::vec4(0.0f, 0.0f, screenSize))) - (screenSize * 0.5f);
}

bool v3d::Camera::isNodeVisible(v3d::TransformNode * node, const v3d::ProjectionType projectionType) const
{
	if (node->isType3D())
	{
		// 3D and 2D both run frustum vs AABB test.

		// @note: When 

		// Check with frustrum
		if (frustum)
		{
			// @todo: Implement this
			//return frustum->isAABBVisible(node->getBoundingBox());
		}

		return false;
	}
	else if (node->isType2D())
	{
		// Check projection type
		if (projectionType == v3d::ProjectionType::ePerspective)
		{
			// Perspective (default)

			// Check with frustrum
			if (frustum)
			{
				// get aabb
				v3d::AABB nodeAABB = node->getBoundingBox();
				// shift to screen space
				nodeAABB.position.z += screenSpaceMatrix[3][2];
				// run frustum vs aabb
				//return frustum->isAABBVisible(nodeAABB);
			}
			// Else, frustum in invalid
		}
		else if (projectionType == v3d::ProjectionType::eOrthographic)
		{
			// Orthographic

			// Check 
		}
	}
	else if (node->isTypeUI())
	{
		// Check projection type
		if (projectionType == v3d::ProjectionType::ePerspective)
		{
			// Perspective

			// Basically same with 2d but frustum without moving
		}
		else if (projectionType == v3d::ProjectionType::eOrthographic)
		{
			// Orthographic (Default)

			// Check if UI is on screen
		}

		//return v3d::CollisionDetection::doesIntersects(screenRectUI, v3d::Rect(node->getBoundingBox()));
	}


	return false;
}

bool v3d::Camera::isNodeVisible(const v3d::TransformNode& node) const
{
	// Check node type
	if (node.isType3D())
	{
		// 3D

		//// Check with frustrum
		//if (frustum)
		//{
		//	// @todo: Implement this
		////	return frustum->isAABBVisible(node.getBoundingBox());
		//}

		// not visible
		return false;
	}
	else if (node.isType2D())
	{
		// 2D 

		// Check projection type
		if (projectionType == v3d::ProjectionType::ePerspective)
		{
			// Perspective

			// Check with frustrum
			if (frustum)
			{
				// get aabb
				v3d::AABB nodeAABB = node.getBoundingBox();

				// shift to screen space
				nodeAABB.position.z += screenSpaceMatrix[3][2];

				// run frustum vs aabb
				//return frustum->isAABBVisible(nodeAABB);
			}
			// Else, frustum is nullptr

			// Not visible
			return false;
		}
		else if (projectionType == v3d::ProjectionType::eOrthographic)
		{
			// Orthographic

			// Check 

			return false;
		}
	}
	else if (node.isTypeUI())
	{
		// UI
		//return v3d::CollisionDetection::doesIntersects(screenRectUI, v3d::Rect(node.getWorldBoundingBox()));
	}
	// else, it's scene

	// Let scene always visible...
	return true;
}

void v3d::Camera::onUpdate(const float delta)
{
	// check
	if (needToUpdateModelMat)
	{
		// update view mat
		updateViewMatrix();
	}

	// call base
	v3d::TransformNode::onUpdate(delta);
}

v3d::NodeType v3d::Camera::getDefaultNodeType() const
{
	return v3d::NodeType::e3D;
}

void v3d::Camera::print(const bool printBase) const
{
	auto& logger = v3d::Logger::getInstance();
	
	// Print camera attribute
	logger.info("[Camera] Camera info...");
	logger.info("[Camera] Projection Type: " + std::string((projectionType == v3d::ProjectionType::ePerspective) ? "Perspective" : "Orthographic"));
	logger.info("[Camera] Name: " + name);
	logger.info("[Camera] Fovy: " + std::to_string(fovy));
	logger.info("[Camera] Fovx: " + std::to_string(fovx));
	logger.info("[Camera] Aspect: " + std::to_string(aspect));
	logger.info("[Camera] Near: " + std::to_string(nears));
	logger.info("[Camera] Far: " + std::to_string(fars));

	// Print frustum
	if (frustum) frustum->print();

	// Print transformation
	v3d::TransformNode::print(printBase);
}