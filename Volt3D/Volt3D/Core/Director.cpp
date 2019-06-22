#include <PreCompiled.h>

#include "Director.h"

#include "Engine.h"
#include "Window.h"
#include "Camera.h"
#include "Utils/Logger.h"

#include "Scene/Scene.h"
//#include "Volt3D/Scene/TransitionScene.h"

#include "Shape/Segment.h"

#include "Input/InputManager.h"

//#include "Volt3D/Renderer/Renderer.h"
//
//#include "Volt3D/Font/FontManager.h"
//#include "Volt3D/Font/Font.h"

#include "Config/BuildConfig.h"
//
//#include "Volt3D/Error/ErrorCodes.h"
//#include "Volt3D/Error/ErrorReport.h"

#if V3D_DEBUG_CAMERA
#include "Volt3D/Debugging/DebugCamera.h"
#endif

v3d::Director::Director(v3d::InputManager & inputManager)
	: runningScene(nullptr)
	, nextScene(nullptr)
	, sceneTransitionState(v3d::Director::SceneTransitionState::IDLE)
	, input(inputManager)
	, needToReplaceScene(false)
	, defaultCamera(nullptr)
#if V3D_DEBUG_CAMERA
	, debugCamera(nullptr)
	, prevCursorMode(v3d::CursorMode::DISABLED)
#endif
{}

v3d::Director::~Director()
{
	if (runningScene)
	{
		runningScene->onExit();
		runningScene->onExitFinished();
		runningScene->onRelease();
		runningScene.reset();
	}

	if (nextScene)
	{
		nextScene->onExit();
		nextScene->onExitFinished();
		nextScene->onRelease();
		nextScene.reset();
	}
}

bool v3d::Director::init(const int resW, const int resH)
{
	return initCameras(resW, resH);
}

bool v3d::Director::initCameras(const int resW, const int resH)
{
	const float resWf = static_cast<float>(resW);
	const float resHf = static_cast<float>(resH);

	// Create default 3D camera
	v3d::Camera* default3DCamera = v3d::Camera::create("Volt3D_Default_3D_Camera", v3d::ProjectionType::ePerspective, 70.0f, 0.01f, 1000.0f, resWf, resHf);

	// check
	if (default3DCamera == nullptr) return false;

	// store
	defaultCamera = (std::move(std::unique_ptr<v3d::Camera>(default3DCamera)));

#if V3D_DEBUG_CAMERA
	if (!initDebugCamera(resWf, resHf)) return false;
#endif

	// Done.
	return true;
}

std::shared_ptr<v3d::Scene> v3d::Director::getRunningScene()
{
	return runningScene;
}

void v3d::Director::runScene(v3d::Scene * newScene)
{
	// Check new scene
	if (newScene == nullptr) return;

	/*
	// Check running scene
	if (runningScene == nullptr)
	{
		// There is no running scene. Make new scene as running scene

		// init scene
		if (!newScene->onInit())
		{
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
			v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::DIRECTOR, v3d::Error::Code::DirectorErrorCode::NEW_SCENE_FAILED_TO_INITIALIZE));
#endif
			// Failed
			return;
		}
		// Else, success.

		// Set to running scene.
		runningScene = std::move(std::shared_ptr<Scene>(newScene));
		
		// Enter
		runningScene->onEnter();

		// Try downcast
		v3d::TransitionScene* transitionScene = dynamic_cast<TransitionScene*>(runningScene.get());

		if (transitionScene)
		{
			// This scene need transition.
			sceneTransitionState = v3d::Director::SceneTransitionState::TRANSITIONING;
		}
		else
		{
			// This scene doesn't need transition.

			// Finished.
			runningScene->onEnterFinished();

			// update scene once
			runningScene->onUpdate(0.0f);

			// Set state to idle
			sceneTransitionState = v3d::Director::SceneTransitionState::IDLE;
		}
	}
	else
	{
		// Else, need to replace scene after running scene's update is done.
		needToReplaceScene = true;

		// store to next scene
		nextScene = std::shared_ptr<v3d::Scene>(newScene);
	}
	*/
}

v3d::Camera & v3d::Director::getDefaultCamera() const
{
	return *defaultCamera;
}

void v3d::Director::update(const float delta)
{
	// Update main camera
	if (defaultCamera) defaultCamera->onUpdate(delta);

#if V3D_DEBUG_CAMERA
	if (debugCamera) debugCamera->update(input, delta);
#endif
	/*
	// Check if need to replace scene before update
	if (needToReplaceScene)
	{
		// Need to replace scene
		
		// Check if scene is running and have next scene
		if (runningScene && nextScene)
		{
			// Have both.

			// Initialize next scene
			if (nextScene->onInit())
			{
				// Init done. Replace current scene with next scene

				// New scene starts to enter
				nextScene->onEnter();

				// Running scene starts to exit
				runningScene->onExit();

				// Try downcast
				v3d::TransitionScene* transitionScene = dynamic_cast<TransitionScene*>(runningScene.get());

				if (transitionScene)
				{
					// This scene need transition.
					sceneTransitionState = v3d::Director::SceneTransitionState::TRANSITIONING;
				}
				else
				{
					// This scene doesn't need transition.

					// next scene finishes entering
					nextScene->onEnterFinished();

					// update scene once
					nextScene->onUpdate(0.0f);

					// running scene finishes exiting
					runningScene->onExitFinished();

					// deref running scene
					runningScene.reset();

					// ref running scene with next scene
					runningScene = nextScene;

					// deref next scene
					nextScene.reset();
					// Make nullptr.
					nextScene = nullptr;

					// Back to idle
					sceneTransitionState = v3d::Director::SceneTransitionState::IDLE;
				}
			}
			else
			{
				// Failed init
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
				v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::DIRECTOR, v3d::Error::Code::DirectorErrorCode::NEXT_SCENE_FAILED_TO_INITIALIZE));
#endif
			}
		}
		// Else, next scene doesn't exist. End of replace.

		// back to false.
		needToReplaceScene = false;
	}
	
	if (sceneTransitionState == v3d::Director::SceneTransitionState::IDLE)
	{
		if (runningScene)
		{
			// Update running scene. 
			
			// @todo: This update is more like pre-update because engine didn't update scene yet.
			//			This means that when user try to access any nodes in the scene, all data is 
			// update user's update function.
			//runningScene->onUpdate(delta);

#if V3D_DEBUG_CAMERA
			if (debugCamera)
			{
				// toggle
				if (input.isKeyPressed(V3D_DEBUG_CAMERA_TOGGLE_KEY, true) && input.isKeyModifierPressing(v3d::KeyModifier::KEY_MOD_ALT | v3d::KeyModifier::KEY_MOD_CONTROL))
				{
					toggleDebugCamera();
				}

				// check
				if (!debugCamera->isActive())
				{
					// Update mouse input for UI objects
					updateUIMouseInput();

					// Then call scene's node update. This will update all children
					runningScene->onUpdate(delta);
				}
			}

#else
			// Update mouse input for UI objects
			updateUIMouseInput();

			// Then call scene's node update. This will update all children
			runningScene->onUpdate(delta);
#endif

			// @todo: post update?
		}
	}
	else
	{
		// transitioning

		if (nextScene)
		{
			nextScene->onUpdate(delta);

			if (nextScene->isTransitionFinished())
			{
				// Transition is finished

				sceneTransitionState = v3d::Director::SceneTransitionState::IDLE;

				// release running scene
				runningScene.reset();

				// make next scene as running scene

				TransitionScene* transitionScene = dynamic_cast<v3d::TransitionScene*>(nextScene.get());

				// must be transition scene
				if (transitionScene)
				{
					runningScene = std::move(std::shared_ptr<Scene>(transitionScene->nextScene));
				}
				// Else, does nothing.

				// Reset next scene
				nextScene.reset();
			}
			// Else, trasition is not finished
		}
		else
		{
			// Next scene is nullptr
			sceneTransitionState = v3d::Director::SceneTransitionState::IDLE;
		}
	}
	*/
}

void v3d::Director::updateUIMouseInput()
{
	// update mouse interaction of scene node and its children.
	/*
	const glm::vec2 mp = input.getMousePosition();

	const glm::vec2 d = input.getMouseMovedDistance();

	runningScene->onUpdateMouseMove(defaultCamera->getRaySegment(mp), d);

	if (input.isAnyMouseButtonPressed(true))
	{
		runningScene->onUpdateMousePress(mp);
	}

	if (input.isAnyMouseButtonReleased(true))
	{
		runningScene->onUpdateMouseRelease(mp);
	}
	*/
}

void v3d::Director::onResolutionChange(const int width, const int height)
{
	// Check is there is running scene
	if (runningScene)
	{
		// It does. Mark entire scene dirty.
		runningScene->markDirty();
	}

	// Update camera
	if (defaultCamera) defaultCamera->updateScreenSizeAndAspect(static_cast<float>(width), static_cast<float>(height));
}

void v3d::Director::render(v3d::Renderer& renderer)
{
	// Check
	if (runningScene)
	{
		// has running scene and renderer is not nullptr

		// Render running scene.
		runningScene->onRender(renderer);
	}
}

void v3d::Director::processRenderer(v3d::Renderer& renderer)
{
	/*
	// check
	if (renderer.isReadyToProcess())
	{
		// valid
#if V3D_DEBUG_CAMERA
		if (debugCamera && debugCamera->isActive())
		{
			// process renderer with debug camera.
			renderer.process(*debugCamera->camera);
		}
		else
		{
			// process with default camera
			renderer.process(*defaultCamera.get());
		}
#else
		// process with default camera
		renderer.process(*defaultCamera.get());
#endif
	}
	*/
}



// ===================== Debug Camera =====================

#if V3D_DEBUG_CAMERA

bool v3d::Director::initDebugCamera(const float resW, const float resH)
{
	// create
	v3d::DebugCamera* newDC = v3d::DebugCamera::create();

	// check
	if (newDC == nullptr) return false;

	// init
	if (!newDC->init(resW, resH)) return false;

	// set
	debugCamera = std::move(std::unique_ptr<v3d::DebugCamera>(newDC));

	// deactivate
	debugCamera->setActive(false);

	return true;
}

void v3d::Director::toggleDebugCamera()
{
	// check
	if (debugCamera) ((debugCamera->isActive() ? disableDebugCamera() : enableDebugCamera()));
}

void v3d::Director::enableDebugCamera()
{
	// check
	if (debugCamera && !debugCamera->isActive())
	{
		// activate
		debugCamera->setActive(true);

		// get glview
		auto& glView = v3d::Engine::getInstance().getGLView();

		// save current cursor mode
		prevCursorMode = glView.getCursorMode();
		// set cursor mode as hidden
		glView.setCursorMode(v3d::CursorMode::DISABLED);
	}
}

void v3d::Director::disableDebugCamera()
{
	// check
	if (debugCamera && debugCamera->isActive())
	{
		// deactivate
		debugCamera->setActive(false);

		// restore old cursor mode
		v3d::Engine::getInstance().getGLView().setCursorMode(prevCursorMode);
	}
}

#endif

// ========================================================