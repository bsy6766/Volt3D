#include <PreCompiled.h>

#include "Frustum.h"

#include "Camera.h"
#include "Shape/AABB.h"
#include "Shape/OBB.h"
#include "Utils/Logger.h"

#if V3D_FRUSTRUM_DEBUG_DRAW
#include "Volt3D/2D/ShapeNode.h"
#endif

v3d::Frustum::Frustum()
#if V3D_FRUSTRUM_DEBUG_DRAW
	: debugDrawShapeNode(nullptr)
#endif
{}

v3d::Frustum::~Frustum() {}

v3d::Frustum * v3d::Frustum::create(const glm::mat4 & VPMat)
{
	// create new intsanace
	v3d::Frustum* newFrustum = new (std::nothrow) v3d::Frustum();

	// check
	if (newFrustum)
	{
		// init
		//newFrustum->updateFrustumPlanes(VPMat);

#if V3D_FRUSTRUM_DEBUG_DRAW
		newFrustum->initDebugShapeNode();
#endif

		// Success.
		return newFrustum;
	}
	else
	{
		// report
		//v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::FRUSTUM);
	}

	// Failed
	return nullptr;
}

/*
void v3d::Frustum::updateFrustumPlanes(const glm::mat4& VPMat)
{
	clippingPlanes.at(v3d::Frustum::ClippingPlane::LEFT).set(glm::vec3(VPMat[0][3] + VPMat[0][0], VPMat[1][3] + VPMat[1][0], VPMat[2][3] + VPMat[2][0]), VPMat[3][3] + VPMat[3][0]);
	clippingPlanes.at(v3d::Frustum::ClippingPlane::RIGHT).set(glm::vec3(VPMat[0][3] - VPMat[0][0], VPMat[1][3] - VPMat[1][0], VPMat[2][3] - VPMat[2][0]), VPMat[3][3] - VPMat[3][0]);
	clippingPlanes.at(v3d::Frustum::ClippingPlane::BOTTOM).set(glm::vec3(VPMat[0][3] + VPMat[0][1], VPMat[1][3] + VPMat[1][1], VPMat[2][3] + VPMat[2][1]), VPMat[3][3] + VPMat[3][1]);
	clippingPlanes.at(v3d::Frustum::ClippingPlane::TOP).set(glm::vec3(VPMat[0][3] - VPMat[0][1], VPMat[1][3] - VPMat[1][1], VPMat[2][3] - VPMat[2][1]), VPMat[3][3] - VPMat[3][1]);
	clippingPlanes.at(v3d::Frustum::ClippingPlane::NEARS).set(glm::vec3(VPMat[0][3] + VPMat[0][2], VPMat[1][3] + VPMat[1][2], VPMat[2][3] + VPMat[2][2]), VPMat[3][3] + VPMat[3][2]);
	clippingPlanes.at(v3d::Frustum::ClippingPlane::FARS).set(glm::vec3(VPMat[0][3] - VPMat[0][2], VPMat[1][3] - VPMat[1][2], VPMat[2][3] - VPMat[2][2]), VPMat[3][3] - VPMat[3][2]);
}

float v3d::Frustum::AABBPlaneCheck(const v3d::AABB & aabb, const v3d::Plane & plane)
{
	const float r = glm::abs(aabb.halfExtents.x * plane.normal.x) + glm::abs(aabb.halfExtents.y * plane.normal.y) + glm::abs(aabb.halfExtents.z * plane.normal.z);
		
	const float d = glm::dot(plane.normal, aabb.position) + plane.distanceToOrigin;

	// return signed distance
	if (glm::abs(d) < r)
	{
		return 0.0f;
	}
	else if (d < 0.0f)
	{
		return d + r;
	}

	return d - r;
}

float v3d::Frustum::OBBPlaneCheck(const v3d::OBB & obb, const v3d::Plane & plane)
{
	const glm::vec3 n = plane.normal * obb.orientation;

	const float r = glm::abs(obb.halfExtents.x * n.x) + glm::abs(obb.halfExtents.y * n.y) + glm::abs(obb.halfExtents.z * n.z);

	const float d = glm::dot(plane.normal, obb.position) + plane.distanceToOrigin;

	// return signed distance
	if (glm::abs(d) < r)
	{
		return 0.0f;
	}
	else if (d < 0.0f)
	{
		return d + r;
	}

	return d - r;
}

bool v3d::Frustum::isAABBVisible(const v3d::AABB & aabb)
{
	// takes 600 ns
	for (auto& plane : clippingPlanes)
	{
		const float side = AABBPlaneCheck(aabb, plane);

		if (side < 0.0f)
		{
			return false;
		}
	}

	return true;
}
*/

/*
bool v3d::Frustum::isAABBVisible2(const v3d::AABB & aabb)
{
	// Takes 600~900ns
	const auto min = aabb.getMin();
	const auto max = aabb.getMax();

	for (int i = 0; i < 6; i++)
	{
		const glm::vec3 p = clippingPlanes[i].normal;
		const float d = clippingPlanes[i].distanceToOrigin;
		int outside = 0;
		outside += (glm::dot(p, glm::vec3(min.x, min.y, min.z)) + d) < 0.f ? 1 : 0;
		outside += (glm::dot(p, glm::vec3(max.x, min.y, min.z)) + d) < 0.f ? 1 : 0;
		outside += (glm::dot(p, glm::vec3(min.x, max.y, min.z)) + d) < 0.f ? 1 : 0;
		outside += (glm::dot(p, glm::vec3(max.x, max.y, min.z)) + d) < 0.f ? 1 : 0;
		outside += (glm::dot(p, glm::vec3(min.x, min.y, max.z)) + d) < 0.f ? 1 : 0;
		outside += (glm::dot(p, glm::vec3(max.x, min.y, max.z)) + d) < 0.f ? 1 : 0;
		outside += (glm::dot(p, glm::vec3(min.x, max.y, max.z)) + d) < 0.f ? 1 : 0;
		outside += (glm::dot(p, glm::vec3(max.x, max.y, max.z)) + d) < 0.f ? 1 : 0;

		if (outside == 8)
		{
			// all points outside the frustum
			return false;
		}
	}

	return true;
}
*/

/*
bool v3d::Frustum::isOBBVisible(const v3d::OBB & obb)
{
	for (auto& plane : clippingPlanes)
	{
		const float side = OBBPlaneCheck(obb, plane);

		if (side < 0.0f) return false;
	}

	return true;
}
*/

void v3d::Frustum::print() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.info("[Frsutum] Info");
	logger.info("Near plane");
	clippingPlanes.at(v3d::Frustum::ClippingPlane::NEARS).print();
	logger.info("Far plane");
	clippingPlanes.at(v3d::Frustum::ClippingPlane::FARS).print();
	logger.info("Left plane");
	clippingPlanes.at(v3d::Frustum::ClippingPlane::LEFT).print();
	logger.info("Right plane");
	clippingPlanes.at(v3d::Frustum::ClippingPlane::RIGHT).print();
	logger.info("Top plane");
	clippingPlanes.at(v3d::Frustum::ClippingPlane::TOP).print();
	logger.info("Bottom plane");
	clippingPlanes.at(v3d::Frustum::ClippingPlane::BOTTOM).print();
}

#if V3D_FRUSTRUM_DEBUG_DRAW

void v3d::Frustum::initDebugShapeNode()
{
	// check
	if (debugDrawShapeNode == nullptr)
	{
		// create
		debugDrawShapeNode = v3d::ShapeNode::create("Frustum Debug Draw");

		// check
		if (debugDrawShapeNode)
		{
			// set as 3D
			debugDrawShapeNode->setNodeType(v3d::NodeType::_3D);
		}
	}
}

void v3d::Frustum::initFrustumDebugLines(const v3d::Camera & camera)
{
	// check
	if (debugDrawShapeNode)
	{
		// clear all
		debugDrawShapeNode->clearAllLines();

		// fov
		const float fovy = camera.getFovy();
		const float fovx = camera.getFovx();
		// near
		const float nears = camera.getNear();
		// far
		const float fars = camera.getFar();

		// compute near pos
		const float nearY = tan(glm::radians(fovy * 0.5f)) * nears;
		const float nearX = tan(glm::radians(fovx * 0.5f)) * nears;

		// build vertex positions
		const glm::vec3 minNear = glm::vec3(-nearX, -nearY, 0.0f);
		const glm::vec3 maxNear = glm::vec3(nearX, nearY, 0.0f);

		const float farY = tan(glm::radians(fovx * 0.5f)) * fars;
		const float farX = tan(glm::radians(fovx * 0.5f)) * fars;

		const glm::vec3 minFar = glm::vec3(-farX, -farY, 0.0f);
		const glm::vec3 maxFar = glm::vec3(farX, farY, 0.0f);
	}
}

void v3d::Frustum::renderDebugDraw()
{
	// check
	if (debugDrawShapeNode)
	{
	}
}

void v3d::Frustum::releaseDebugDraw()
{
	if (debugDrawShapeNode)
	{
		delete debugDrawShapeNode;
		debugDrawShapeNode = nullptr;
	}
}

#endif

#if V3D_DEBUG_FRUSTUM_LINE
void Voxel::Frustum::initDebugLines(const float fovy, const float fovx, const float near, const float far)
{
	std::vector<float> vertices;

	float nearY = tan(glm::radians(fovy*0.5f)) * near;
	float nearX = tan(glm::radians(fovx*0.5f)) * near;

	glm::vec2 minNear = glm::vec2(-nearX, -nearY);
	glm::vec2 maxNear = glm::vec2(nearX, nearY);

	float farY = tan(glm::radians(fovx*0.5f)) * far;
	float farX = tan(glm::radians(fovx*0.5f)) * far;

	glm::vec2 minFar = glm::vec2(-farX, -farY);
	glm::vec2 maxFar = glm::vec2(farX, farY);

	// Near left bot > left top > right top > right bottom (clock wise)
	vertices.push_back(minNear.x);
	vertices.push_back(minNear.y);
	vertices.push_back(-near);

	vertices.push_back(minNear.x);
	vertices.push_back(maxNear.y);
	vertices.push_back(-near);

	vertices.push_back(maxNear.x);
	vertices.push_back(maxNear.y);
	vertices.push_back(-near);

	vertices.push_back(maxNear.x);
	vertices.push_back(minNear.y);
	vertices.push_back(-near);

	// same for far
	vertices.push_back(minFar.x);
	vertices.push_back(minFar.y);
	vertices.push_back(-far);

	vertices.push_back(minFar.x);
	vertices.push_back(maxFar.y);
	vertices.push_back(-far);

	vertices.push_back(maxFar.x);
	vertices.push_back(maxFar.y);
	vertices.push_back(-far);

	vertices.push_back(maxFar.x);
	vertices.push_back(minFar.y);
	vertices.push_back(-far);

	std::vector<float> color;

	for (int i = 0; i < 8; i++)
	{
		color.push_back(0);
		color.push_back(0);
		color.push_back(1);
		color.push_back(1);
	}

	std::vector<unsigned int> indices = { 0,1, 1,2, 2,3, 3,0, 4,5, 5,6, 6,7, 7,4, 0,4, 1,5, 2,6, 3,7 };

	// Generate vertex array object
	glGenVertexArrays(1, &vao);
	// Bind it
	glBindVertexArray(vao);

	GLuint vbo;
	// Generate buffer object
	glGenBuffers(1, &vbo);
	// Bind it
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);

	auto program = ProgramManager::getInstance().getProgram(ProgramManager::PROGRAM_NAME::LINE_SHADER);

	GLint vertLoc = program->getAttribLocation("vert");
	GLint colorLoc = program->getAttribLocation("color");

	// vert
	glEnableVertexAttribArray(vertLoc);
	glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	GLuint cbo;
	// Generate buffer object
	glGenBuffers(1, &cbo);
	// Bind it
	glBindBuffer(GL_ARRAY_BUFFER, cbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * color.size(), &color.front(), GL_STATIC_DRAW);

	// vert
	glEnableVertexAttribArray(colorLoc);
	glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	// Bind indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	// Load indices
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices.front(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	// Delte buffers
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &cbo);
	glDeleteBuffers(1, &ibo);
}

void Voxel::Frustum::render(const glm::mat4 & modelMatrix, Program* prog)
{
	if (vao)
	{
		prog->setUniformMat4("modelMat", modelMatrix);
		prog->setUniformVec4("lineColor", glm::vec4(1.0f));

		glBindVertexArray(vao);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	}
}
#endif
