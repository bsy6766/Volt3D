/**
*	@file Scene.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_SCENE_H
#define V3D_SCENE_H

#include <string>

#include "Node/TransformNode.h"
#include "Utils/Macros.h"

namespace v3d
{
	/**
	*	@class Scene
	*	@brief Base class for all scene.
	*
	*	This class is abstract class for all scenes.
	*/
	class VOLT3D_DLL Scene : public v3d::TransformNode
	{
		friend class Director;

	protected:
		// constructor with name
		Scene(const std::string& name);

	public:
		//  destructor
		virtual ~Scene();

		/**
		*	Initialize scene.
		*	Called by director when scene is added to director.
		*/
		virtual bool onInit() = 0;

		/**
		*	Release everything in the scene.
		*	This is called after onExitFinished() and before destruction.
		*/
		virtual void onRelease() = 0;

		/**
		*	Called when scene starts to enter.
		*	If scene was transitioning, this is called when transition effect for this scene starts.
		*	Else, this will be called after init() if there isn't any transition.
		*/
		virtual void onEnter() = 0;

		/**
		*	Called when scene finishes enter.
		*	If scene was transitioing, this is called when transition effect ends for this scene.
		*	Else, this will be called after onEnter().
		*/
		virtual void onEnterFinished() = 0;

		/**
		*	Called when scene starts to exit.
		*	Scene exits when another scene enters.
		*	If new scene has transition, this is called when transition effect starts.
		*	Else, this will be called when new scene is pushed.
		*/
		virtual void onExit() = 0;

		/**
		*	Called when scene finishes exit.
		*	If scene was transitioning, this is called when transition ends for this scene.
		*	Else, this will be called after onExit().
		*/
		virtual void onExitFinished() = 0;

		/**
		*	Called when scene transition is finished.
		*	Only called when scene was transitioning. 
		*	@return true if scene transitioning is finished.
		*/
		virtual bool isTransitionFinished();

		/**
		*	Get default node type.
		*	Scene's default node type is Scene.
		*	@return v3d::NodeType::_SCENE
		*/
		GET_DEFULT_NODE_TYPE(v3d::NodeType::eScene);
	};
}

#endif