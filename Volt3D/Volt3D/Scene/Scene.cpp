#include <PreCompiled.h>

#include "Scene.h"

#include "Volt3D/Core/Camera.h"

v3d::Scene::Scene(const std::string & name)
	: v3d::TransformNode(name, v3d::NodeType::eScene)
{
	// Scene is always visible
	visible = true;
}

v3d::Scene::~Scene() {}

bool v3d::Scene::isTransitionFinished()
{
	// Transition is always finished for scenes.
	// Transition scenes overrides this and gives different result.
	return true;
}
