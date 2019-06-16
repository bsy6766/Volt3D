#ifndef V3D_TRANSFORM_NODE_H
#define V3D_TRANSFORM_NODE_H

#include <string>
#include <list>
#include <vector>
#include <memory>
#include <functional>

#include "BaseNode.h"

#include "ZOrder.h"

//#include "Volt3D/Action/Action.h"

#include "Shape/AABB.h"

#include "NodeType.h"
#include "Pivot.h"

//#include "Volt3D/Enums/ProjectionType.h"
//#include "Volt3D/Config/DebugConfig.h"

#include "Utils/Macros.h"

namespace v3d
{
	// foward declaration
	class Camera;
	class Segment;
	class Action;
	class Renderer;
	class TransformNode;
	class RenderCommand;
	namespace UI
	{
		class Button;
		class Checkbox;
		class NinePatchSprite;
		class Slider;
		class TextField;
		class ScrollNode;
	}
#if V3D_DRAW_BOUNDING_BOX
	class ShapeNode;
#endif

	/**
	*	@class TransformNode
	*	@brief A node that can be transformed.
	*
	*	Any object that exists in world coordinate system needs to derive TransformNode.
	*	This class contains attributes for transformation matrix.
	*/
	class VOLT3D_DLL TransformNode : public BaseNode
	{
		friend class Director;
		friend class Scene;
		friend class RenderNode;
		friend class Node;
		friend class Sprite;
		friend class SpriteAnimation;
		friend class ProgressTimer;
		friend class ParticleSystem;
		friend class Text;
		friend class UI::Button;
		friend class UI::Checkbox;
		friend class UI::NinePatchSprite;
		friend class UI::Slider;
		friend class UI::TextField;
		friend class UI::ScrollNode;
#if V3D_DEBUG_CONSOLE
		friend class DebugConsole;
#endif

	protected:
		// updates model matrix if it's true
		bool needToUpdateModelMat;

		// Calls sortChildren() if it's true
		bool needToSortChildren;

		// Sorts children 
		void sortChildren();

		// set content size
		void setContentSize2D(const glm::vec2& contentSize);
		void setContentSize3D(const glm::vec3& contentSize);
	protected:
		// ===================== ctor =======================

		// Constructor with name and type. 
		TransformNode(const std::string& name, const v3d::NodeType type);
		// ==================================================

		// Type of node
		v3d::NodeType nodeType;

		// Parent of this node.
		v3d::TransformNode* parent;

		// Children of this node. 
		std::vector<v3d::TransformNode*> children;

		// interaction flag
		bool interactable;

		// Visible state. true if node is visible from camera. Else, false.
		bool visible;

		// Z order
		v3d::ZOrder zOrder;

		// ================ Transformation ==================

		/**
		*	Position
		*	The position of the object.
		*	2D and UI mainly uses x and y values, but z value can be used as well.
		*/
		glm::vec3 position;

		/**
		*	Rotation
		*	A euler rotation in degree angle.
		*/
		glm::vec3 rotation;	

		/**
		*	Scale
		*	The scale of the object.
		*/
		glm::vec3 scale;

		/**
		*	Pivot [-0.5f, 0.5f]
		*	Default is set to 0.0f for all axes.
		*	Sets the center point of object. 
		*	i.e.) Setting pivot to (0.0f, 0.5f, 0.0f) will have center point
		*	on top center of object.
		*/
		glm::vec3 pivot;

		/**
		*	Anchor [-0.5f, 0.5f]
		*	Default is set to 0.0f for all axes.
		*	Sets the center point of parent object.
		*	i.e.) If UI Canvas is positioned center of screen with size of 100 x 100,
		*	and chld UI object has anchor of (0.0f, 0.5f, 0.0f) and position (0.0f, 0.0f, 0.0f)
		*	child UI object will be rendered on (0.0f, 100.0f, 0.0f) instead of actual position.
		*/
		glm::vec3 anchor;

		/**
		*	Content size
		*	The original size of content without scale.
		*	For 2D objects, z value will remain 0.0f.
		*/
		glm::vec3 contentSize;

		/**
		*	Model matrix
		*	World model matrix.
		*	To get local model matrix, use getModelMatrix().
		*/
		glm::mat4 modelMatrix;
		// ==================================================

		/**
		*	Bounding box (AABB)
		*	A bounding box of the object.
		*	The values are set to world space.
		*/
		v3d::AABB aabb;

		// ==================== Actions =====================

		// List of actions running.
		std::list<std::shared_ptr<v3d::Action>> actions;

		// Pause flag.
		bool actionPaused;
		// ==================================================
		

		// get parent matrix. Returns parent's model matrix with coordinate origin applied
		glm::mat4 getParentMatrix() const;

		// calculate model matrix
		virtual glm::mat4 getModelMatrix();

		// ================ Mouse callbacks =================
		std::function<void(v3d::TransformNode & sender)> onMousePressed;
		std::function<void(v3d::TransformNode & sender)> onMouseReleased;
		std::function<void(v3d::TransformNode & sender)> onMouseEnter;
		std::function<void(v3d::TransformNode & sender)> onMouseExit;
		std::function<void(v3d::TransformNode & sender)> onMouseMove;
		// ==================================================
		

		// ======= Mouse update function for UI objects =====
		virtual bool onUpdateMouseMoveSelf(const v3d::Segment& raySegment, const glm::vec2& mouseDelta);
		virtual bool onUpdateMouseMove(const v3d::Segment& raySegment, const glm::vec2& mouseDelta);
		virtual bool onUpdateMousePress(const glm::vec2& mousePosition);
		virtual bool onUpdateMouseRelease(const glm::vec2& mousePositionn);
		virtual void onUpdateMouseMoveFalse();
		virtual void onUpdateMouseMoveFalseAll();
		virtual void onUpdateMousePressFalse();
		virtual void onUpdateMousePressFalseAll();
		// ==================================================


		void checkChildrenSort();
		void checkModelMatrixUpdate();


	public:
		// Destructor
		virtual ~TransformNode();


		// ===================== Type ========================

		// Get type of node
		v3d::NodeType getNodeType() const;
		// Check if node type is 3D
		bool isType3D() const;
		// Check if node type is 2D
		bool isType2D() const;
		// Check if node type is UI
		bool isTypeUI() const;
		// Check if node type is Scene
		bool isTypeScene() const;
		// Get default type
		virtual v3d::NodeType getDefaultNodeType() const = 0;

		/**
		*	Set node type to this node.
		*	Node type doesn't mean it forces node's type to certain type.
		*	For example, 2D or UI can still be trnaformed with z axis, etc.
		*	NodeType decide how node will be projected, batched and rendered.
		*
		*	@param nodeType A new node type to set.
		*	@warning Undefined node will be rejected.
		*/
		virtual bool setNodeType(const v3d::NodeType nodeType);

		/**
		*	Set node type and also to all children.
		*
		*	@param nodeType A new node type to set.
		*	@warning Undefined node will be rejected.
		*/
		virtual bool setNodeTypeCascade(const v3d::NodeType nodeType);
		
		// ==================================================

		/**
		*	Set visibility.
		*	Only visible nodes are rendered.
		*	Invisible nodes are still updated.
		*	@param visibility true to set node visible. Else, false.
		*/
		virtual void setVisibility(const bool visibility);

		/**
		*	Check if node is visible
		*	@return true if node is visible. Else, false.
		*/
		bool isVisible() const;

		/**
		*	Set x position.
		*	@param x x position to set.
		*/
		void setPositionX(const float x);

		/**
		*	Set y position.
		*	@param y y position to set.
		*/
		void setPositionY(const float y);

		/**
		*	Set z position.
		*	@param z z position to set.
		*/
		void setPositionZ(const float z);

		/**
		*	Set xy position.
		*	@param x x position to set.
		*	@param y y position to set.
		*/
		void setPosition(const float x, const float y);

		/**
		*	Set xyz position.
		*	@param x x position to set.
		*	@param y y position to set.
		*	@param z z position to set.
		*/
		void setPosition(const float x, const float y, const float z);

		/**
		*	Set xy position.
		*	@param position A new vec2 position to set.
		*/
		void setPosition(const glm::vec2& position);

		/**
		*	Set xyz position.
		*	@param position A new vec3 position to set.
		*/
		void setPosition(const glm::vec3& position);

		/**
		*	Add x position.
		*	@param x An amount of x position to add on current position.
		*/
		void addPositionX(const float x);

		/**
		*	Add y position.
		*	@param y An amount of y position to add on current position.
		*/
		void addPositionY(const float y);

		/**
		*	Add z position.
		*	@param z An amount of z position to add on current position.
		*/
		void addPositionZ(const float z);

		/**
		*	Add xy position.
		*	@param delta A amount of position to add on current position.
		*/
		void addPosition(const glm::vec2& delta);

		/**
		*	Add xyz position.
		*	@param delta A amount of position to add on current position.
		*/
		void addPosition(const glm::vec3& delta);

		// Get x position of node
		float getPositionX() const;

		// Get y position of node
		float getPositionY() const;

		// Get z position of node
		float getPositionZ() const;

		// Get xy position of node.
		glm::vec3 getPosition() const;

		/**
		*	Set rotation angle of node in x axis
		*	@param angle An amount of angle in degrees to set in x axis.
		*/
		void setRotationX(const float angle);

		/**
		*	Set rotation angle of node in y axis
		*	@param angle An amount of angle in degrees to set in y axis.
		*/
		void setRotationY(const float angle);

		/**
		*	Set rotation angle of node in z axis
		*	@param angle An amount of angle in degrees to set in z axis.
		*/
		void setRotationZ(const float angle);

		/**
		*	Set rotation angle of node in xy axis
		*	@param angle An amount of angle in degrees to set in xy axis.
		*/
		void setRotation(const float xAngle, const float yAngle);

		/**
		*	Set rotation angle of node in xyz axis
		*	@param angle An amount of angle in degrees to set in xyz axis.
		*/
		void setRotation(const float xAngle, const float yAngle, const float zAngle);

		/**
		*	Set rotation angle of node in xy axis.
		*	@param angle An amount of angle in degrees to set in xy axis.
		*/
		void setRotation(const glm::vec2& angle);

		/**
		*	Set rotation angle of node in xyz axis.
		*	@param angle An amount of angle in degrees to set in xyz axis.
		*/
		void setRotation(const glm::vec3& angle);

		/**
		*	Add rotation angle of node in x axis.
		*	@param angle An amount of angle in degrees to add in x axis.
		*/
		void addRotationX(const float angle);

		/**
		*	Add rotation angle of node in y axis.
		*	@param angle An amount of angle in degrees to add in y axis.
		*/
		void addRotationY(const float angle);

		/**
		*	Add rotation angle of node in z axis.
		*	@param angle An amount of angle in degrees to add in z axis.
		*/
		void addRotationZ(const float angle);

		/**
		*	Add rotation angle of node in xy axis.
		*	@param angle An amount of angle in degrees to add in xy axis.
		*/
		void addRotation(const glm::vec2& angle);

		/**
		*	Add rotation angle of node in xyz axis.
		*	@param angle An amount of angle in degrees to add in xyz axis.
		*/
		void addRotation(const glm::vec3& angle);

		// Get rotation of node in x axis
		float getRotationX() const;

		// Get rotation of node in y axis
		float getRotationY() const;

		// Get rotation of node in z axis
		float getRotationZ() const;

		// Get rotation of node
		glm::vec3 getRotation() const;

		/**
		*	Set scale of node in x axis
		*	@param scale Scale value for x axis.
		*/
		void setScaleX(const float scale);

		/**
		*	Set scale of node in y axis
		*	@param scale Scale value for y axis
		*/
		void setScaleY(const float scale);

		/**
		*	Set scale of node in z axis
		*	@param scale Scale value for z axis
		*/
		void setScaleZ(const float scale);

		/**
		*	Set scale of node in xyz axis to same value.
		*	@param scale Scale for all xyz axis
		*/
		void setScale(const float scale);

		/**
		*	Set scale.
		*	@param xScale Scale in x axis.
		*	@param yScale Scale in y axis.
		*/
		void setScale(const float xScale, const float yScale);

		/**
		*	Set scale.
		*	@param xScale Scale in x axis.
		*	@param yScale Scale in y axis.
		*	@param zScale Scale in z axis.
		*/
		void setScale(const float xScale, const float yScale, const float zScale);

		/**
		*	Set scale of node in xy axis.
		*	@param scale Scale value of x and y axis.
		*/
		void setScale(const glm::vec2& scale);

		/**
		*	Set scale of node in all xyz axis.
		*	@param scale Scale in all xyz axis.
		*/
		void setScale(const glm::vec3& scale);

		/**
		*	Add scale of node in x axis
		*	@param xScale Scale value of x axis.
		*/
		void addScaleX(const float xScale);

		/**
		*	Add scale of node in y axis
		*	@param yScale Scale value of y axis.
		*/
		void addScaleY(const float yScale);

		/**
		*	Add scale of node in z axis
		*	@param zScale Scale value of z axis.
		*/
		void addScaleZ(const float zScale);

		/**
		*	Add scale of node in xy axis
		*	@param xScale Scale value of x axis.
		*	@param yScale Scale value of y axis.
		*/
		void addScale(const float xScale, const float yScale);

		/**
		*	Add scale of node in xy axis
		*	@param Scale value of x and y axis.
		*/
		void addScale(const glm::vec2& scale);

		/**
		*	Add scale of node in xy axis
		*	@param xScale Scale value of x axis.
		*	@param yScale Scale value of y axis.
		*	@parma zScale Scale value of z axis.
		*/
		void addScale(const float xScale, const float yScale, const float zScale);

		/**
		*	Add scale of node in xy axis
		*	@param Scale value of xyz axis
		*/
		void addScale(const glm::vec3& scale);

		// Get scale of node.
		glm::vec3 getScale() const;

		/**
		*	Set pivot in x axis
		*	@param xPivot A pivot value of x axis 
		*/
		void setPivotX(const float xPivot);

		/**
		*	Set pivot in y axis
		*	@param yPivot A pivot value of y axis 
		*/
		void setPivotY(const float yPivot);

		/**
		*	Set pivot in z axis
		*	@param zPivot A pivot value of z axis 
		*/
		void setPivotZ(const float zPivot);

		/**
		*	Set pivot in xy axis
		*	@param pivot Pivot to set. 
		*/
		void setPivot(const glm::vec2& pivot);

		/**
		*	Set pivot in xy axis
		*	@param xPivot A pivot value of x axis
		*	@param yPivot A pivot value of y axis
		*/
		void setPivot(const float xPivot, const float yPivot);

		/**
		*	Set pivot in xyz axis
		*	@param pivot Pivot to set
		*/
		void setPivot(const glm::vec3& pivot);

		/**
		*	Set pivot in xyz axis
		*	@param xPivot A pivot value of x axis
		*	@param yPivot A pivot value of y axis
		*	@param zPivot A pivot value of z axis 
		*/
		void setPivot(const float xPivot, const float yPivot, const float zPivot);
		
		/**
		*	Set pivot
		*	@param pivot A predefined pivot enum.
		*	@see v3d::Pivot for details.
		*/
		void setPivot(const v3d::Pivot pivot);

		// Get pivot of node
		glm::vec3 getPivot() const;

		/**
		*	Set anchor in x axis
		*	@param xAnchor A anchor value of x axis
		*/
		void setAnchorX(const float xAnchor);

		/**
		*	Set anchor in y axis
		*	@param yAnchor A anchor value of y axis
		*/
		void setAnchorY(const float yAnchor);

		/**
		*	Set anchor in z axis
		*	@param zAnchor A anchor value of z axis
		*/
		void setAnchorZ(const float zAnchor);

		/**
		*	Set anchor in xy axis
		*	@param anchor Anchor to set.
		*/
		void setAnchor(const glm::vec2& anchor);

		/**
		*	Set anchor in xy axis
		*	@param xAnchor A anchor value of x axis
		*	@param yAnchor A anchor value of y axis
		*/
		void setAnchor(const float xAnchor, const float yAnchor);

		/**
		*	Set anchor in xyz axis
		*	@param anchor Anchor to set
		*/
		void setAnchor(const glm::vec3& anchor);

		/**
		*	Set anchor in xyz axis
		*	@param xAnchor A anchor value of x axis
		*	@param yAnchor A anchor value of y axis
		*	@param zAnchor A anchor value of z axis
		*/
		void setAnchor(const float xAnchor, const float yAnchor, const float zAnchor);
		
		// Get anchor of node
		glm::vec3 getAnchor() const;

		// Get content size of this node. Content size is original size of node, without getting affected by scale.
		glm::vec3 getContentSize() const;
		
		// Get bounidng box of this node.
		v3d::AABB getBoundingBox() const;

		/**
		*	Get world bounding box of this node.
		*	This applies parent matrix to current AABB, 
		*	so it makes the AABB in world position instead of local node position.
		*	If there is no parent on this node, there is no point of doing this.
		*	@return A world space AABB of this node.
		*/
		v3d::AABB getWorldBoundingBox() const;
		
		/**
		*	Set local Z order.
		*	Z order is only affected by 2D and screen space UI objects.
		*	3D objects aren't affected by z order.
		*	@param localZOrder A new local z order to set.
		*/
		void setLocalZOrder(const int localZOrder);

		/**
		*	Set global Z order.
		*	Negative global z order is rendered before 0 and positive global z order.
		*	Z order is only affected by 2D and screen space UI objects.
		*	3D objects aren't affected by z order.
		*	@param globalZOrder A new global z order to set.
		*/
		void setGlobalZOrder(const int globalZOrder);

		/**
		*	Set z order with ZOrder.
		*	Z order is only affected by 2D and screen space UI objects.
		*	3D objects aren't affected by z order.
		*	@param zOrder A new z order to set.
		*/
		void setZorder(const v3d::ZOrder& zOrder);

		// Get z order of this node. 
		v3d::ZOrder getZOrder() const;

		/**
		*	Set node iteractable or non-interactable.
		*	Interactable nodes will get mouse update and will call few mouse update callbacks.
		*	Non-interactable nodes will not get mouse update.
		*	@param interactable true to set interactable. fals to set non-interactable.
		*/
		void setInteractable(const bool interactable);
		
		/**
		*	Check if node is interactable.
		*	@return true if it's interactable. Else, false.
		*/
		bool isInteractable() const;

		/**
		*	Add child to this node.
		*	Because you didn't specify the z order of this node,
		*	it will set global zorder to 0. Local zorder will be 0
		*	if there isn't any child on this node. Else, it will be
		*	same with the last child among children.
		*
		*	@param child A raw pointer to child node.
		*
		*	@return true if successfully added child. Else, false.
		*/
		bool addChild(v3d::TransformNode* child);

		/**
		*	Add child with local z order to this node.
		*	Because you didn't specify the global z order of this node,
		*	it will set global zorder to 0.
		*
		*	@param child A raw pointer to child node.
		*	@param localZOrder Local z order to set.
		*
		*	@return true if successfully added child. Else, false.
		*/
		bool addChild(v3d::TransformNode* child, const int localZOrder);

		/**
		*	Add child with ZOrder.
		*
		*	@param child A raw pointer to child node.
		*	@param zOrder z order to set.
		*
		*	@return true if successfully added child. Else, false.
		*/
		virtual bool addChild(v3d::TransformNode* child, const v3d::ZOrder& zOrder);

		/**
		*	Remove child by name. Searches the name and remvoes from children.
		*/
		bool removeChild(const std::string& name, const bool releaseChild = false);

		/**
		*	Remove child by id. Searches the id and remvoes from children.
		*/
		bool removeChild(const unsigned int id, const bool releaseChild = false);

		/**
		*	Remove child by instance.
		*	This attemps to find by z order. If fails, searcehs with id.
		*/
		bool removeChild(v3d::TransformNode* child, const bool releaseChild = false);

		bool removeAllChildren( const bool cleanup = false );

		// Check if node has parent
		bool hasParent() const;

		/**
		*	Set parent.
		*	
		*	@param parent A parent node to set. If parent is nullptr, does nothing. To remove parent @see removeParent()
		*/
		void setParent(v3d::TransformNode* parent);

		/**
		*	Get parent.
		*	@return Parent if has one. Else, nullptr.
		*/
		v3d::TransformNode* getParent() const;
		
		/**
		*	Removes parent and removes itself from parent.
		*/
		void removeParent();

		/**
		*	Get child by name. This isn't efficient way since it uses string key.
		*	Use this for debug or incase where performance doesn't matter.
		*	@param name Name.
		*	@return First child that matches the name. nullptr if doesn't exists
		*/
		v3d::TransformNode* getChild(const std::string& name);

		/**
		*	Get all children of this node.
		*	@return Vector of children in ascending order of ZOrder (low to high)
		*/
		std::vector<v3d::TransformNode*> getChildren() const;

		/**
		*	Get all children of this node.
		*	@return Vector reference of children in ascending order of ZOrder (low to high)
		*/
		const std::vector<v3d::TransformNode*>& getChildrenRef() const;

		/**
		*	Check if this node has children
		*	@return true if has children. Else, false.
		*/
		bool hasChildren() const;
		
		/**
		*	Update actions
		*	This is called once every frame.
		*	@param delta Elapsed time for current frame.
		*/
		//void updateActions(const float delta);

		/**
		*	Update children.
		*	Iterate all children of this node and update.
		*	@param delta Elapsed time for current frame.
		*/
		void updateChildren(const float delta);
		
		/**
		*	Update node.
		*	This updates everything needs for this node.
		*	@param delta Elapsed time for current frame
		*/
		virtual void onUpdate(const float delta);

		/**
		*	Updates model matrix.
		*	This will update model marix based on node's tranformation and parent's matrix.
		*	This function will update model matrix itself and all children aswell and it's called whenever transformation (translation, rotation, scale, pivot, etc) changes on self or any parents to root parent.
		*/
		virtual void updateModelMatrix();
		
		/**
		*	Updates bounding box
		*	This will update the bounding box to screen space, not node space.
		*	This function will only update bounding box itself but children. Also it's called whenever transformation (translation, rotation, scale, pivot, etc) changes on self or any parents to root parent.
		*/
		virtual void updateBoundingBox();
				
		/**
		*	Set callback function for mouse press.
		*	This callback is called when mouse is pressed on node.
		*	v3d::MouseButton param on callback tells which mouse button was clicked
		*	@param func Callback func.
		*/
		void setOnMousePressedCallback(const std::function<void(v3d::TransformNode & sender)>& func);

		/**
		*	Set callback function for mouse release
		*	This callback is called when mouse is released.
		*	First parameter tells which button was released. Second parameter tells if it was released on node.
		*	This callback is called only if mouse was pressed on the node at the first place.
		*	@param func Callback func.
		*/
		void setOnMouseReleasedCallback(const std::function<void(v3d::TransformNode & sender)>& func);

		/**
		*	Set callback function for mouse enter.
		*	This callback is called when mouse enters the node bounding box.
		*	@param func Callback func
		*/
		void setOnMouseEnterCallback(const std::function<void(v3d::TransformNode & sender)>& func);

		/**
		*	Set callback function for mouse exit.
		*	This callback is called when mouse exits the node bounding box.
		*	@param func Callback func
		*/
		void setOnMouseExitCallback(const std::function<void(v3d::TransformNode & sender)>& func);

		/**
		*	Set callback function for mouse move.
		*	This callback function is called when mouse moves in node's bounding box
		*	@param func Callback func
		*/
		void setOnMouseMoveCallback(const std::function<void(v3d::TransformNode & sender)>& func);

		/**
		*	Mark this node dirty.
		*	Making node dirty will make node to update all transformation
		*	and bounding box.
		*	All children in this node gets affected as well.
		*/
		void markDirty();
		
		/**
		*	Queue action and run.
		*	@param action An action to run.
		*/
		//void runAction(v3d::Action* action);

		/**
		*	Pause all actions.
		*/
		//void pauseAllActions();

		/**
		*	Resume all actions.
		*/
		//void resumeAllActions();

		/**
		*	Restart all actions.
		*/
		//void restartAllActions();
		
		/**
		*	Clear all actions in this node.
		*/
		//void clearAllActions();

		/**
		*	Get total action count.
		*	@return Total number of actio
		*/
		//std::size_t getTotalActionsCount() const;

		/**
		*	Check if this node is running action.
		*/
		//bool isRunningAction() const;

		/**
		*	Render this transform node.
		*	This is the function called during the rendering loop every frame.
		*	@param renderer A renderer ref passed by the game engine.
		*/
		virtual void onRender(v3d::Renderer& renderer);

		/**
		*	Log TransformNode information.
		*/
		virtual void print(const bool printBase) const override;

		/**
		*	Log all children in this node.
		*	@param tab Leave this as 0 (default).
		*/
		void printChildren(const int tab = 0) const;


		// Debug draw
#if V3D_DRAW_BOUNDING_BOX
		// The reason why transform node has debug shape draw while it has
		// nothing to render?
		// Because transform node is the node that contains bounding box and
		// some objects that doesn't render at all can have something to show.
		// For example, a v3d::Node that has custom content size can still
		// show bounding box.
	public:
		v3d::ShapeNode* debugDrawShapeNode;
		void initDebugShapeNode(const v3d::NodeType nodeType);
		void releaseDebugShapeNode();
		void updateDebugDraw();
		virtual void updateDebugDrawShapeNode();
#endif
	};




	// struct for children sort
	struct ChildrenSortCmp
	{
		bool operator()(const v3d::TransformNode* lhs, const v3d::TransformNode* rhs) const
		{
			const v3d::ZOrder& left = lhs->getZOrder();
			const v3d::ZOrder& right = rhs->getZOrder();

			// Check global z order
			if (left.getGlobalZOrder() == right.getGlobalZOrder())
			{
				// same global z order. Compare local z order
				return left.getLocalZOrder() < right.getLocalZOrder();
			}
			else
			{
				// Not same global z order. Compare global z order
				return left.getGlobalZOrder() < right.getGlobalZOrder();
			}
		}
	};
}

#endif