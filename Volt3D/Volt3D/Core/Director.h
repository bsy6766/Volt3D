#ifndef VOLT3D_DIRECTOR_H
#define VOLT3D_DIRECTOR_H

#include <memory>

#include <glm/glm.hpp>

#include "CursorMode.h"
#include "ProjectionType.h"
#include "Utils/Macros.h"

namespace v3d
{
	// Foward declaration
	class Scene;
	class Camera;
	class InputManager;
	class Renderer;
#if V3D_DEBUG_CAMERA
	class DebugCamera;
#endif

	/**
	*	@class Director
	*	@brief Manages scenes in the game. Look Scene for more info.
	*
	*	Director is a class that manages all the scenes in the game.
	*	Multiple scenes can be stored in Director but only one scene can be run and updated by Director.
	*/
	class VOLT3D_DLL Director
	{
		friend class Engine;
		friend class Camera;

	private:
		enum class SceneTransitionState
		{
			IDLE = 0,			// update and renders current scene if there is current scene
			TRANSITIONING,		// Transition between current scene and next scene
		};
		
	private:
		// Constructor
		Director() = delete;
		Director(v3d::InputManager& inputManager);

		std::shared_ptr<v3d::Scene> runningScene;
		std::shared_ptr<v3d::Scene> nextScene;
		
		// State
		v3d::Director::SceneTransitionState sceneTransitionState;

		v3d::InputManager& input;

		// Default camera
		std::unique_ptr<v3d::Camera> defaultCamera;

		// flag
		bool needToReplaceScene;

		// Initialize
		bool init(const int resW, const int resH);

		// Initialize main cmaera
		bool initCameras(const int resW, const int resH);
		
		// Update mouse input for UI objects
		void updateUIMouseInput();

		// Update all that is related to screen size
		void onResolutionChange(const int width, const int height);

	public:
		// Destructor
		~Director();

		// Get running scene
		std::shared_ptr<v3d::Scene> getRunningScene() const;
		
		/**
		*	Run scene.
		*	@param scene A scene to run. If newScene is transition scene, transition will run first.
		*/
		void runScene( std::shared_ptr<v3d::Scene> scene );

		/**
		*	Get default camera
		*	@return A default v3d::Camera instance reference.
		*/
		v3d::Camera& getDefaultCamera() const;

		/**
		*	Update director.
		*	@param delta Elapsed time on current frame.
		*/
		void update(const float delta);

		// Queue render commands to renderer.
		void render(v3d::Renderer& renderer);

		// process render queue and draw
		void processRenderer(v3d::Renderer& renderer);


		// ===================== Debug Camera =====================
#if V3D_DEBUG_CAMERA
	private:
		std::unique_ptr<v3d::DebugCamera> debugCamera;
		v3d::CursorMode prevCursorMode;
		bool initDebugCamera(const float resW, const float resH);
	public:
		void toggleDebugCamera();
		void enableDebugCamera();
		void disableDebugCamera();
#endif
		// ========================================================
	};

}
#endif