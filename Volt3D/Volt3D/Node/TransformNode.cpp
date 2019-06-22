#include <PreCompiled.h>

#include "TransformNode.h"

#include "Utils/Logger.h"
#include "Config/BuildConfig.h"
#include "Action/Action.h"

//#include "Volt3D/Shape/Rect.h"
//#include "Volt3D/Shape/Segment.h"
//#include "Volt3D/Utility/String.h"

#if V3D_DRAW_BOUNDING_BOX
#include "Volt3D/2D/ShapeNode.h"
#include "Volt3D/Renderer/Renderer.h"
#include "Volt3D/RenderCommand/PrimitiveCommand.h"
#endif

v3d::TransformNode::TransformNode(const std::string & name, const v3d::NodeType type)
	: v3d::BaseNode(name)
	, nodeType(type)
	, visible(true)
	, position(0.0f)
	, rotation(0.0f)
	, scale(1.0f)
	, pivot(0.0f)
	, anchor(0.0f)
	, contentSize()
	, modelMatrix(1.0f)
	, zOrder()
	, aabb()
	, needToUpdateModelMat(true)
	, needToSortChildren(true)
	, parent(nullptr)
	, interactable(false)
	, actionPaused(false)
	, onMousePressed(nullptr)
	, onMouseReleased(nullptr)
	, onMouseEnter(nullptr)
	, onMouseExit(nullptr)
	, onMouseMove(nullptr)
#if V3D_DRAW_BOUNDING_BOX
	, debugDrawShapeNode(nullptr)
#endif
{}

v3d::TransformNode::~TransformNode()
{
	// clear actions
	actions.clear();

	// delete all children
	for (auto child : children)
	{
		if (child)
		{
			delete child;
		}
	}

	children.clear();

#if V3D_DRAW_BOUNDING_BOX
	releaseDebugShapeNode();
#endif
}

v3d::NodeType v3d::TransformNode::getNodeType() const
{
	return nodeType;
}

bool v3d::TransformNode::isType3D() const
{
	return nodeType == v3d::NodeType::e3D;
}

bool v3d::TransformNode::isType2D() const
{
	return nodeType == v3d::NodeType::e2D;
}

bool v3d::TransformNode::isTypeUI() const
{
	return nodeType == v3d::NodeType::eGUI;
}

bool v3d::TransformNode::isTypeScene() const
{
	return nodeType == v3d::NodeType::eScene;
}

void v3d::TransformNode::setPositionX(const float x)
{
	position.x = x;

	markDirty();
}

void v3d::TransformNode::setPositionY(const float y)
{
	position.y = y;

	markDirty();
}

void v3d::TransformNode::setPositionZ(const float z)
{
	position.z = z;

	markDirty();
}

void v3d::TransformNode::setPosition(const float x, const float y)
{
	position.x = x;
	position.y = y;

	markDirty();
}

void v3d::TransformNode::setPosition(const float x, const float y, const float z)
{
	position.x = x;
	position.y = y;
	position.z = z;

	markDirty();
}

void v3d::TransformNode::setPosition(const glm::vec2 & position)
{
	setPosition(position.x, position.y);
}

void v3d::TransformNode::setPosition(const glm::vec3 & position)
{
	setPosition(position.x, position.y, position.z);
}

void v3d::TransformNode::addPositionX(const float x)
{
	position.x += x;

	markDirty();
}

void v3d::TransformNode::addPositionY(const float y)
{
	position.y += y;

	markDirty();
}

void v3d::TransformNode::addPositionZ(const float z)
{
	position.z += z;

	markDirty();
}

void v3d::TransformNode::addPosition(const glm::vec2 & delta)
{
	position.x += delta.x;
	position.y += delta.y;

	markDirty();
}

void v3d::TransformNode::addPosition(const glm::vec3 & delta)
{
	position += delta;

	markDirty();
}

float v3d::TransformNode::getPositionX() const
{
	return position.x;
}

float v3d::TransformNode::getPositionY() const
{
	return position.y;
}

float v3d::TransformNode::getPositionZ() const
{
	return position.z;
}

glm::vec3 v3d::TransformNode::getPosition() const
{
	return position;
}

void v3d::TransformNode::setRotationX(const float angle)
{
	rotation.x = angle;

	markDirty();
}

void v3d::TransformNode::setRotationY(const float angle)
{
	rotation.y = angle;

	markDirty();
}

void v3d::TransformNode::setRotationZ(const float angle)
{
	rotation.z = angle;

	markDirty();
}

void v3d::TransformNode::setRotation(const float xAngle, const float yAngle)
{
	rotation.x = xAngle;
	rotation.y = yAngle;

	markDirty();
}

void v3d::TransformNode::setRotation(const float xAngle, const float yAngle, const float zAngle)
{
	rotation.x = xAngle;
	rotation.y = yAngle;
	rotation.z = zAngle;

	markDirty();
}

void v3d::TransformNode::setRotation(const glm::vec2 & angle)
{
	setRotation(angle.x, angle.y);
}

void v3d::TransformNode::setRotation(const glm::vec3 & angle)
{
	setRotation(angle.x, angle.y, angle.z);
}

void v3d::TransformNode::addRotationX(const float angle)
{
	rotation.x += angle;

	markDirty();
}

void v3d::TransformNode::addRotationY(const float angle)
{
	rotation.y += angle;

	markDirty();
}

void v3d::TransformNode::addRotationZ(const float angle)
{
	rotation.z += angle;

	markDirty();
}

void v3d::TransformNode::addRotation(const glm::vec2 & angle)
{
	rotation.x += angle.x;
	rotation.y += angle.y;

	markDirty();
}

void v3d::TransformNode::addRotation(const glm::vec3 & angle)
{
	rotation.x += angle.x;
	rotation.y += angle.y;
	rotation.z += angle.z;

	markDirty();
}

float v3d::TransformNode::getRotationX() const
{
	return rotation.x;
}

float v3d::TransformNode::getRotationY() const
{
	return rotation.y;
}

float v3d::TransformNode::getRotationZ() const
{
	return rotation.z;
}

glm::vec3 v3d::TransformNode::getRotation() const
{
	return rotation;
}

void v3d::TransformNode::setScaleX(const float scale)
{
	this->scale.x = scale;

	markDirty();
}

void v3d::TransformNode::setScaleY(const float scale)
{
	this->scale.y = scale;

	markDirty();
}

void v3d::TransformNode::setScaleZ(const float scale)
{
	this->scale.z = scale;

	markDirty();
}

void v3d::TransformNode::setScale(const float scale)
{
	this->scale = glm::vec3(scale);

	markDirty();
}

void v3d::TransformNode::setScale(const float xScale, const float yScale)
{
	scale.x = xScale;
	scale.y = yScale;

	markDirty();
}

void v3d::TransformNode::setScale(const float xScale, const float yScale, const float zScale)
{
	scale.x = xScale;
	scale.y = yScale;
	scale.z = zScale;

	markDirty();
}

void v3d::TransformNode::setScale(const glm::vec2 & scale)
{
	setScale(scale.x, scale.y);
}

void v3d::TransformNode::setScale(const glm::vec3 & scale)
{
	setScale(scale.x, scale.y, scale.z);
}

void v3d::TransformNode::addScaleX(const float xScale)
{
	scale.x += xScale;

	markDirty();
}

void v3d::TransformNode::addScaleY(const float yScale)
{
	scale.y += yScale;

	markDirty();
}

void v3d::TransformNode::addScaleZ(const float zScale)
{
	scale.z += zScale;

	markDirty();
}

void v3d::TransformNode::addScale(const float xScale, const float yScale)
{
	scale.x += xScale;
	scale.y += yScale;

	markDirty();
}

void v3d::TransformNode::addScale(const glm::vec2 & scale)
{
	addScale(scale.x, scale.y);
}

void v3d::TransformNode::addScale(const float xScale, const float yScale, const float zScale)
{
	scale.x += xScale;
	scale.y += yScale;
	scale.z += zScale;

	markDirty();
}

void v3d::TransformNode::addScale(const glm::vec3 & scale)
{
	addScale(scale.x, scale.y, scale.z);
}

glm::vec3 v3d::TransformNode::getScale() const
{
	return scale;
}

void v3d::TransformNode::setPivotX(const float xPivot)
{
	pivot.x = xPivot;

	markDirty();
}

void v3d::TransformNode::setPivotY(const float yPivot)
{
	pivot.y = yPivot;

	markDirty();
}

void v3d::TransformNode::setPivotZ(const float zPivot)
{
	pivot.z = zPivot;

	markDirty();
}

void v3d::TransformNode::setPivot(const glm::vec2 & pivot)
{
	setPivot(pivot.x, pivot.y);
}

void v3d::TransformNode::setPivot(const float xPivot, const float yPivot)
{
	this->pivot.x = xPivot;
	this->pivot.y = yPivot;

	markDirty();
}

void v3d::TransformNode::setPivot(const glm::vec3 & pivot)
{
	setPivot(pivot.x, pivot.y, pivot.z);
}

void v3d::TransformNode::setPivot(const float xPivot, const float yPivot, const float zPivot)
{
	this->pivot.x = xPivot;
	this->pivot.y = yPivot;
	this->pivot.z = zPivot;

	markDirty();
}

void v3d::TransformNode::setPivot(const v3d::Pivot pivot)
{
	switch (pivot)
	{
	case v3d::Pivot::eCenter:
		this->pivot.x = 0.0f;
		this->pivot.y = 0.0f;
		this->pivot.z = 0.0f;
		break;
	case v3d::Pivot::eLeft:
		this->pivot.x = -0.5f;
		this->pivot.y = 0.0f;
		break;
	case v3d::Pivot::eRight:
		this->pivot.x = 0.5f;
		this->pivot.y = 0.0f;
		break;
	case v3d::Pivot::eTop:
		this->pivot.x = 0.0f;
		this->pivot.y = 0.5f;
		break;
	case v3d::Pivot::eBottom:
		this->pivot.x = 0.0f;
		this->pivot.y = -0.5f;
		break;
	case v3d::Pivot::eLeftTop:
		this->pivot.x = -0.5f;
		this->pivot.y = 0.5f;
		break;
	case v3d::Pivot::eLeftBottom:
		this->pivot.x = -0.5f;
		this->pivot.y = -0.5f;
		break;
	case v3d::Pivot::eRightTop:
		this->pivot.x = 0.5f;
		this->pivot.y = 0.5f;
		break;
	case v3d::Pivot::eRightBottom:
		this->pivot.x = 0.5f;
		this->pivot.y = -0.5f;
		break;
	default:
		// return in case of wrong pivot
		return;
		break;
	}

	markDirty();
}

glm::vec3 v3d::TransformNode::getPivot() const
{
	return pivot;
}

void v3d::TransformNode::setAnchorX(const float xAnchor)
{
	this->anchor.x = xAnchor;

	markDirty();
}

void v3d::TransformNode::setAnchorY(const float yAnchor)
{
	this->anchor.y = yAnchor;

	markDirty();
}

void v3d::TransformNode::setAnchorZ(const float zAnchor)
{
	this->anchor.z = zAnchor;

	markDirty();
}

void v3d::TransformNode::setAnchor(const glm::vec2 & anchor)
{
	setAnchor(anchor.x, anchor.y);
}

void v3d::TransformNode::setAnchor(const float xAnchor, const float yAnchor)
{
	this->anchor.x = xAnchor;
	this->anchor.y = yAnchor;

	markDirty();
}

void v3d::TransformNode::setAnchor(const glm::vec3 & anchor)
{
	setAnchor(anchor.x, anchor.y, anchor.z);
}

void v3d::TransformNode::setAnchor(const float xAnchor, const float yAnchor, const float zAnchor)
{
	this->anchor.x = xAnchor;
	this->anchor.y = yAnchor;
	this->anchor.z = zAnchor;

	markDirty();
}

glm::vec3 v3d::TransformNode::getAnchor() const
{
	return anchor;
}

void v3d::TransformNode::updateBoundingBox()
{
	// Builds AABB based on content size and scale.
	
	// get half size.
	const glm::vec3 halfSize = contentSize * 0.5f;

	// get 4 points (bottom left, top left, top right, bottom right) with model matrix applied.
	const glm::vec4 points[4] =
	{
		modelMatrix * glm::vec4(-halfSize, 1.0f),
		modelMatrix * glm::vec4(-halfSize.x, halfSize.y, halfSize.z, 1.0f),
		modelMatrix * glm::vec4(halfSize, 1.0f),
		modelMatrix * glm::vec4(halfSize.x, -halfSize.y, halfSize.z, 1.0f)
	};

	// Get min max
	const glm::vec4 min = glm::min(points[0], glm::min(points[1], glm::min(points[2], points[3])));
	const glm::vec4 max = glm::max(points[0], glm::max(points[1], glm::max(points[2], points[3])));

	// set aabb half extent
	aabb.halfExtents = glm::abs(glm::vec3(max - min) * 0.5f);

	// update aabb position.
	// Apply pivot.

	// Get how much we have to shift by pivot
	const glm::vec3 pivotShift = contentSize * pivot * scale;

	// make new position
	glm::vec3 newAABBPos = position;

	// shift by pivot
	newAABBPos.x -= pivotShift.x;
	newAABBPos.y -= pivotShift.y;

	// Only 3D
	if (nodeType == v3d::NodeType::e3D)
	{
		// shifting object by pivot in Z axis only works in 3D
		newAABBPos.z -= pivotShift.z;
	}

	// apply position
	aabb.position = newAABBPos;

	// Done.
}

v3d::AABB v3d::TransformNode::getBoundingBox() const
{
	return aabb;
}

v3d::AABB v3d::TransformNode::getWorldBoundingBox() const
{
	// copy aabb
	v3d::AABB aabbcopy = aabb;
	
	// get world position
	aabbcopy.position = getParentMatrix() * glm::vec4(aabbcopy.position, 1.0f);

	// done.
	return aabbcopy;
}

void v3d::TransformNode::setLocalZOrder(const int localZOrder)
{
	zOrder.setLocalZOrder(localZOrder);
}

void v3d::TransformNode::setGlobalZOrder(const int globalZOrder)
{
	zOrder.setGlobalZOrder(globalZOrder);
}

void v3d::TransformNode::setZorder(const v3d::ZOrder & zOrder)
{
	this->zOrder = zOrder;

	needToSortChildren = true;
}

v3d::ZOrder v3d::TransformNode::getZOrder() const
{
	return zOrder;
}

void v3d::TransformNode::setInteractable(const bool interactable)
{
	this->interactable = interactable;
}

bool v3d::TransformNode::isInteractable() const
{
	return interactable;
}

bool v3d::TransformNode::addChild(v3d::TransformNode * child)
{
	if (children.empty())
	{
		// children is empty. Z order is set to 0 for both global and local
		return addChild(child, v3d::ZOrder());
	}
	else
	{
		// Use same local z order. Global remains 0.
		return addChild(child, v3d::ZOrder(0, children.back()->getZOrder().getLocalZOrder()));
	}
}

bool v3d::TransformNode::addChild(v3d::TransformNode * child, const int localZOrder)
{
	// Add child with given local z order
	return addChild(child, v3d::ZOrder(localZOrder));
}

bool v3d::TransformNode::addChild(v3d::TransformNode * child, const v3d::ZOrder& zOrder)
{
	// @optimize: If we are adding new child at the back, doesn't need to sort.

	// Check if child has parent. If so, can't add child that has parent
	//if (child->hasParent()) return false;

	// Check if this is scene node
	if (isTypeScene() == false)
	{
		// Not a scene.

		// @todo: Check all scenarios for adding different types of node to different types of parent node.

		// set type. All children follows parent's type.
		child->setNodeTypeCascade(nodeType);
	}
	// Else, this is scene. Keep node's type.

	// set z order
	child->zOrder.setGlobalZOrder(zOrder.getGlobalZOrder());
	child->zOrder.setLocalZOrder(zOrder.getLocalZOrder());

	// check parent
	if (child->hasParent())
	{
		// remove parent
		child->removeParent();
	}

	// set parent
	child->parent = this;

	// add new children to back.
	children.push_back(child);

	// Sort children. 
	needToSortChildren = true;

	return true;
}

bool v3d::TransformNode::removeChild(const std::string & name, const bool releaseChild)
{
	for (auto child_it = std::begin(children); child_it != std::end(children); ++child_it)
	{
		if (*child_it)
		{
			if ((*child_it)->name == name)
			{
				children.erase(child_it);

				return true;
			}
		}
	}

	return false;
}

bool v3d::TransformNode::removeChild(const unsigned int id, const bool releaseChild)
{
	for (auto child_it = std::begin(children); child_it != std::end(children); ++child_it)
	{
		if (*child_it)
		{
			if ((*child_it)->id == id)
			{
				children.erase(child_it);

				return true;
			}
		}
	}

	return false;
}

bool v3d::TransformNode::removeChild(TransformNode * child, const bool releaseChild)
{
	for (auto child_it = std::begin(children); child_it != std::end(children); ++child_it)
	{
		if (*child_it)
		{
			if ((*child_it)->id == child->id && (*child_it)->name == child->name)
			{
				children.erase(child_it);

				return true;
			}
		}
	}

	return false;
}

bool v3d::TransformNode::removeAllChildren( const bool cleanup )
{
	if( cleanup )
	{
		for( auto child : children )
		{
			if( child ) delete child;
			child = nullptr;
		}
	}
	
	children.clear();
	return true;
}

bool v3d::TransformNode::hasParent() const
{
	return parent != nullptr;
}

void v3d::TransformNode::setParent(TransformNode * parent)
{
	if (parent)
	{
		// set parent
		if (this->parent)
		{
			// parent exists. remove from children
			this->parent->removeChild(this);
			// set to nullptr
			this->parent = nullptr;
		}

		// add to new parent
		parent->addChild(this);
	}
}

v3d::TransformNode * v3d::TransformNode::getParent() const
{
	return parent;
}

void v3d::TransformNode::removeParent()
{
	// remove parent.
	if (this->parent)
	{
		// parent exists. remove from children
		this->parent->removeChild(this);
		// set to nullptr
		this->parent = nullptr;
	}
}

void v3d::TransformNode::markDirty()
{
	needToUpdateModelMat = true;

	for (auto child : children)
	{
		if (child)
		{
			child->markDirty();
		}
	}
}

void v3d::TransformNode::sortChildren()
{
	if (!children.empty())
	{
		std::stable_sort(std::begin(children), std::end(children), v3d::ChildrenSortCmp());
	}
}

void v3d::TransformNode::setContentSize2D(const glm::vec2 & contentSize)
{
	this->contentSize.x = contentSize.x;
	this->contentSize.y = contentSize.y;
	this->contentSize.z = 0.0f;

	markDirty();
}

void v3d::TransformNode::setContentSize3D(const glm::vec3 & contentSize)
{
	this->contentSize = contentSize;

	markDirty();
}

v3d::TransformNode * v3d::TransformNode::getChild(const std::string & name)
{
	for (auto child : children)
	{
		if (child)
		{
			if (child->getName() == name)
			{
				return child;
			}
		}
	}

	return nullptr;
}

std::vector<v3d::TransformNode*> v3d::TransformNode::getChildren() const
{
	// return copy.
	return children;
}

const std::vector<v3d::TransformNode*>& v3d::TransformNode::getChildrenRef() const
{
	return children;
}

bool v3d::TransformNode::hasChildren() const
{
	return !(children.empty());
}

glm::vec3 v3d::TransformNode::getContentSize() const
{
	return contentSize;
}

glm::mat4 v3d::TransformNode::getParentMatrix() const
{
	if (parent)
	{
		// @optimize: If parent's content size is 0 (i.e. v3d::Node), doesn't have to compute translation.
		return parent->modelMatrix * glm::translate(glm::mat4(1.0f), parent->getContentSize() * parent->getScale() * anchor);
	}
	else
	{
		return glm::mat4(1.0f);
	}
}

glm::mat4 v3d::TransformNode::getModelMatrix()
{
	// Get model matrix

	// create new matrix
	glm::mat4 mat(1.0f);

	// translate first
	mat = glm::translate(mat, position);

	// rotate
	mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	// We use minus on z axis because of camera's view point. Positive rotation rotates right and negative rotation rotates left from camera's view point.
	mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, -1.0f));

	// translate pivot
	mat = glm::translate(mat, pivot * contentSize * -1.0f * scale);

	// scale
	mat = glm::scale(mat, scale);

	return mat;
}

bool v3d::TransformNode::setNodeType(const v3d::NodeType nodeType)
{
	if (this->nodeType != nodeType)
	{
		this->nodeType = nodeType;

#if V3D_DRAW_BOUNDING_BOX
		if (debugDrawShapeNode) debugDrawShapeNode->setNodeType(nodeType);
#endif

		return true;
	}

	return false;
}

bool v3d::TransformNode::setNodeTypeCascade(const v3d::NodeType nodeType)
{
	if (this->nodeType != nodeType)
	{
		this->nodeType = nodeType;

		// Recursively set child to same type
		for (auto child : children)
		{
			if (child)
			{
				child->setNodeType(nodeType);
			}
		}

#if V3D_DRAW_BOUNDING_BOX
		if (debugDrawShapeNode) debugDrawShapeNode->setNodeTypeCascade(nodeType);
#endif

		return true;
	}

	return false;
}

void v3d::TransformNode::updateModelMatrix()
{
	// Update model matrix
	if (parent)
	{
		// multiply model matrix with parent's
		modelMatrix = getParentMatrix() * getModelMatrix();
	}
	else
	{
		modelMatrix = getModelMatrix();
	}
}

void v3d::TransformNode::updateActions(const float delta)
{
	// Don't update if action is paused or actions are empty.
	if (actionPaused || actions.empty()) return;

	// Iterate actions.
	// If actions is done, remove from actions.
	
	// Get iterator
	auto it = std::begin(actions);

	// Iterate until end.
	for (; it != std::end(actions);)
	{
		// Check if it's done.
		if ((*it)->isDone())
		{
			// Action is done.
			// v3d::Repeat returns true when actually repeating in done
			// v3d::RepeatForever never returns true.
			// So don't have to worry about these.

			// Remove it from action list
			it = actions.erase(it);
		}
		else
		{
			// Action is still running.
			(*it)->update(delta);
			it++;
		}
	}
}

void v3d::TransformNode::updateChildren(const float delta)
{
	// Iterate children
	for (auto child : children)
	{
		// check
		if (child)
		{
			// Update
			child->onUpdate(delta);
		}
	}
}

void v3d::TransformNode::checkChildrenSort()
{
	// Check
	if (needToSortChildren)
	{
		// sort children
		sortChildren();

		// done
		needToSortChildren = false;
	}
}

void v3d::TransformNode::checkModelMatrixUpdate()
{
	if (needToUpdateModelMat)
	{
		// Update model matrix
		updateModelMatrix();
		// Update bounding box based on model mat
		updateBoundingBox();
		// mark as false.
		needToUpdateModelMat = false;

#if V3D_DRAW_BOUNDING_BOX
		if (debugDrawShapeNode)
		{
			// update debug draw
			updateDebugDrawShapeNode();

			// Call this separately because some class override above function.
			updateDebugDraw();
		}
#endif
	}
}

void v3d::TransformNode::onUpdate(const float delta)
{
	// Only active nodes are updated
	if (active == false) return;

	// Check if children need to sort
	checkChildrenSort();

	// update actions
	//updateActions(delta);

	// Check if this object needs to updat model matrix
	checkModelMatrixUpdate();

	// Update children
	updateChildren(delta);
}

bool v3d::TransformNode::onUpdateMouseMoveSelf(const v3d::Segment & raySegment, const glm::vec2 & mouseDelta)
{
	// @warning: override this!
	return false;
}

bool v3d::TransformNode::onUpdateMouseMove(const v3d::Segment & raySegment, const glm::vec2 & mouseDelta)
{
	// Update mouse move for this node
	if (active && visible)
	{
		// active and visible

		// Check children
		if (children.empty())
		{
			// No children. update self.
			return onUpdateMouseMoveSelf(raySegment, mouseDelta);
		}
		else
		{
			// Has children
			bool childHovered = false;

			// 1) Reverse iterate children to update from highest positive global z order to lowest.

			// iterators
			const auto cEnd = children.rend();
			auto rit = children.rbegin();

			// iterate
			for (; rit != cEnd && (*rit)->getZOrder().getGlobalZOrder() >= 0;)
			{
				// Update current child
				bool hovered = (*rit)->onUpdateMouseMove(raySegment, mouseDelta);

				// Check
				if (hovered)
				{
					// There was a child hovered.
					childHovered = true;
					break;
				}
				else
				{
					// Mouse didn't hover child
					rit++;
				}
			}

			// 2) Check if there was a child hovered
			if (childHovered)
			{
				// If so, iterate remaining child and mark as false.

				// Increment iterator to next.
				rit++;
				// Iterate
				for (; rit != cEnd;)
				{
					// Update as mouse move false
					(*rit)->onUpdateMouseMoveFalse();
					// Cascade
					(*rit)->onUpdateMouseMoveFalseAll();

					// next
					rit++;
				}

				// And self aswell
				onUpdateMouseMoveFalse();

				// return as true
				return true;
			}

			// 3) Mouse didn't hover any child with positive global z order. Check self.
			bool selfHovered = onUpdateMouseMoveSelf(raySegment, mouseDelta);

			// Check
			if (selfHovered)
			{
				// If so, iterate remaining child and mark as false.

				// Iterate
				for (; rit != cEnd;)
				{
					// Update as mouse move false
					(*rit)->onUpdateMouseMoveFalse();
					// Cascade
					(*rit)->onUpdateMouseMoveFalseAll();

					// next
					rit++;
				}

				// Return as true
				return true;
			}
			else
			{
				// Else, didn't hovered. Mark as false.

				// todo: Check if this is really needed.
				onUpdateMouseMoveFalse();
			}

			// 4) Mouse didn't hover any child with positive global z order and self. Check children with negative z order

			// Iterate
			for (; rit != cEnd;)
			{
				// Update current child
				bool hovered = (*rit)->onUpdateMouseMove(raySegment, mouseDelta);
				// Check
				if (hovered)
				{
					// There was a child hovered.
					childHovered = true;
					break;
				}
				else
				{
					// Mouse didn't hover child
					rit++;
				}
			}

			// 5) Check if there was a child hovered with negative global z  order
			if (childHovered)
			{
				// If so, iterate remaining child and mark as false.

				// Increment iterator to next.
				rit++;
				// Iterate
				for (; rit != cEnd;)
				{
					// Update as mouse move false
					(*rit)->onUpdateMouseMoveFalse();
					// Cascade
					(*rit)->onUpdateMouseMoveFalseAll();

					// next
					rit++;
				}

				// Return as true
				return true;
			}
			// Else, mouse didn't move over any.

			// return as false
			return false;
		}
	}
	// Else, not visible

	return false;
}

bool v3d::TransformNode::onUpdateMousePress(const glm::vec2 & mousePosition)
{
	// Only active and visible node can be pressed
	if (active && visible)
	{
		// check children
		if (children.empty() == false)
		{
			// Iterate in reverse order to update children that has higher global z order

			// result
			bool childPressed = false;

			// Create reverse iterator of children
			auto rit = children.rbegin();
			auto rend = children.rend();

			// Iterate children
			for (; rit != rend;)
			{
				// update mouse move
				childPressed = (*rit)->onUpdateMousePress(mousePosition);

				// Check
				if (childPressed)
				{
					// Mouse was pressed on one of children node. Stop iterating.
					break;
				}
				else
				{
					// Mouse did not press on current child node. keep iterate.
					rit++;
				}
			}
			// end of iteration.

			// Check result
			if (childPressed)
			{
				// One of children was pressed.

				// Set self as failed
				onUpdateMousePressFalse();

				// point to next
				rit++;

				// iterate rest of 
				for (; rit != rend; ++rit)
				{
					// Update rest of children as press fail.
					(*rit)->onUpdateMousePressFalseAll();
				}
				// end of iteration

				// Done. Return true because there was child pressed.
				return true;
			}
			// else, not hovered
		}
	}
	// Else, deactivated or invisible

	// There was no children that mouse pressed
	return false;
}

bool v3d::TransformNode::onUpdateMouseRelease(const glm::vec2 & mousePosition)
{
	// Only active and visible node can be released
	if (active && visible)
	{
		// check children
		if (children.empty() == false)
		{
			// Iterate in reverse order to update children that has higher global z order

			// result
			bool childReleased = false;

			// Create reverse iterator of children
			auto rit = children.rbegin();
			auto rend = children.rend();

			// Iterate children
			for (; rit != rend;)
			{
				// update mouse move
				childReleased = (*rit)->onUpdateMouseRelease(mousePosition);

				// Check
				if (childReleased)
				{
					// Mouse was released on one of children node. Stop iterating.
					return true;
				}
				else
				{
					// Mouse did not release on current child node. keep iterate.
					rit++;
				}
			}
			// end of iteration.
		}
	}
	// Else, deactivated or invisible

	// There was no children that mouse released
	return false;
}

void v3d::TransformNode::onUpdateMouseMoveFalse()
{
	// Does nothing if didn't override!
	return;
}

void v3d::TransformNode::onUpdateMouseMoveFalseAll()
{
	if (children.empty() == false)
	{
		// Get reverse iterator
		auto rit = children.rbegin();
		// Get reverse end
		const auto rend = children.rend();

		// iterate
		for (; rit != rend; ++rit)
		{
			// nullptr guard
			if ((*rit))
			{
				// Update as mouse move false
				(*rit)->onUpdateMouseMoveFalse();
				// Cascade
				(*rit)->onUpdateMouseMoveFalseAll();
			}
		}
	}
}

void v3d::TransformNode::onUpdateMousePressFalse()
{
	//auto rit = children.rbegin();
	//const auto rit_end = children.rend();

	//for (; rit != rit_end; ++rit)
	//{
	//	if ((*rit))
	//	{
	//		(*rit)->onUpdateMousePressFalse();
	//	}
	//}

	// Does nothing if didn't override!
	return;
}

void v3d::TransformNode::onUpdateMousePressFalseAll()
{
	// Get reverse iterator
	auto rit = children.rbegin();
	// Get reverse end
	const auto rend = children.rend();

	// iterate
	for (; rit != rend; ++rit)
	{
		// nullptr guard
		if ((*rit))
		{
			// Update as mouse press false
			(*rit)->onUpdateMousePressFalse();
			// Cascade
			(*rit)->onUpdateMousePressFalseAll();
		}
	}
}

void v3d::TransformNode::setVisibility(const bool visibility)
{
	visible = visibility;
}

bool v3d::TransformNode::isVisible() const
{
	return visible;
}

void v3d::TransformNode::setOnMousePressedCallback(const std::function<void(v3d::TransformNode &)>& func)
{
	onMousePressed = func;
}

void v3d::TransformNode::setOnMouseReleasedCallback(const std::function<void(v3d::TransformNode &)>& func)
{
	onMouseReleased = func;
}

void v3d::TransformNode::setOnMouseEnterCallback(const std::function<void(v3d::TransformNode &)>& func)
{
	onMouseEnter = func;
}

void v3d::TransformNode::setOnMouseExitCallback(const std::function<void(v3d::TransformNode &)>& func)
{
	onMouseExit = func;
}

void v3d::TransformNode::setOnMouseMoveCallback(const std::function<void(v3d::TransformNode &)>& func)
{
	onMouseMove = func;
}

bool v3d::TransformNode::runAction(std::shared_ptr<v3d::Action> action)
{
	if (action == nullptr) return false;
	if (action)
	{
		actions.push_back(std::shared_ptr<v3d::Action>(action));
		actions.back()->setTarget(this);
		return true;
	}
}

void v3d::TransformNode::clearAllActions()
{
	actions.clear();
}

std::size_t v3d::TransformNode::getTotalActionsCount() const
{
	return actions.size();
}

bool v3d::TransformNode::isRunningAction() const
{
	return (!actions.empty()) && (!actionPaused);
}

void v3d::TransformNode::pauseAllActions()
{
	actionPaused = true;
}

void v3d::TransformNode::resumeAllActions()
{
	actionPaused = false;
}

void v3d::TransformNode::restartAllActions()
{
	for (auto& action : actions)
	{
		if (action)
		{
			action->reset();
		}
	}
}

void v3d::TransformNode::onRender(v3d::Renderer& renderer)
{
	// Transform node render.
	// TransformNode doesn't have anything to render itself.
	// All renderable nodes inherits RenderNode and RenderNode's
	// onRender override redefines it.

	// Check
	if (active && visible)
	{
		// node is active

#if V3D_DRAW_BOUNDING_BOX
	// If rendering success and shapenode is not nullptr, render debug bounding box.
		if (debugDrawShapeNode) renderer.queueDebugDraw(debugDrawShapeNode->linePrimitiveCommand.get());
#endif

		// check
		if (children.empty())
		{
			// There is no children to render
			return;
		}
		else
		{
			// has children

			// Iterate
			for (auto child : children)
			{
				// check
				if (child)
				{
					// render
					child->onRender(renderer);
				}
			}
		}
	}
}

void v3d::TransformNode::print(const bool printBase) const
{
	auto& logger = v3d::Logger::getInstance();

	logger.info("[TransformNode] Position: " + glm::to_string(position));
	logger.info("[TransformNode] Rotation: " + glm::to_string(rotation));
	logger.info("[TransformNode] Scale: " + glm::to_string(scale));

	if (printBase) v3d::BaseNode::print(false);
}

void v3d::TransformNode::printChildren(const int tab) const
{
	auto& logger = v3d::Logger::getInstance();

	std::string tabStr = "";

	for (int i = 0; i < tab; i++)
	{
		tabStr += "\t";
	}

	for (auto child : children)
	{
		if (child)
		{
			zOrder.print();
			logger.info(tabStr + ", Name: " + child->getName());

			child->printChildren(tab + 1);
		}
	}
}



#if V3D_DRAW_BOUNDING_BOX
void v3d::TransformNode::initDebugShapeNode(const v3d::NodeType nodeType)
{
	if (debugDrawShapeNode == nullptr)
	{
		debugDrawShapeNode = v3d::ShapeNode::create(name + "DebugDraw");

		if (debugDrawShapeNode == nullptr)
		{
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
			v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::RENDER_NODE, v3d::Error::Code::RNErrCode::FAILED_TO_INIT_DEBUG_DRAW_SHAPE_NODE));
#endif
		}

		debugDrawShapeNode->setNodeType(nodeType);
	}
}

void v3d::TransformNode::releaseDebugShapeNode()
{
	if (debugDrawShapeNode)
	{
		delete debugDrawShapeNode;
		debugDrawShapeNode = nullptr;
	}
}

void v3d::TransformNode::updateDebugDraw()
{
	if (debugDrawShapeNode) debugDrawShapeNode->onUpdate(0.0f);
}

void v3d::TransformNode::updateDebugDrawShapeNode()
{
	// check
	if (debugDrawShapeNode)
	{
		// check
		if (nodeType == v3d::NodeType::e3D)
		{
			// check x, y, and z.
			if (contentSize.x == 0.0f && contentSize.y == 0.0f && contentSize.z == 0.0f) 
			{
				// nothing to draw
				return;
			}
		}
		else if (nodeType != v3d::NodeType::_SCENE)
		{
			// 2D and UI
			if (contentSize.x == 0.0f && contentSize.y == 0.0f)
			{
				// nothing to draw
				return;
			}
		}

		// copy aabb
		v3d::AABB aabbcopy = aabb;

		// get parent matrix
		const glm::mat4 parentMatrix = getParentMatrix();

		// get world position
		aabbcopy.position = parentMatrix * glm::vec4(aabbcopy.position, 1.0f);

		// get all points from aabb copy
		std::array<glm::vec3, 8> aabbPoints = aabbcopy.getPoints();

		// Draw (Front to back, starting bottom left, clock wise. And then connection between front and back)
		debugDrawShapeNode->clearAllLines();
		debugDrawShapeNode->addAABB(aabbcopy, V3D_DEBUG_DRAW_COLOR);
	}
}
#endif
