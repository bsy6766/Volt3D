/**
*	@file Volt3D.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef VOLT3D_H
#define VOLT3D_H

// Action
#include "Action/Action.h"

// Color
#include "Color/Color3B.h"
#include "Color/Color3F.h"
#include "Color/Color4B.h"
#include "Color/Color4F.h"

// Config
#include "Config/BuildConfig.h"
#include "Config/DebugConfig.h"
#include "Config/GLFWCallbackConfig.h"

// Debug
#include "Debug/DebugCamera.h"

// Engine
#include "Engine/Application.h"
#include "Engine/Camera.h"
#include "Engine/CursorMode.h"
#include "Engine/Director.h"
#include "Engine/Engine.h"
#include "Engine/Frustum.h"
#include "Engine/Preference.h"
#include "Engine/ProjectionType.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "Engine/WindowMode.h"

// Input
#include "Input/GamepadAxis.h"
#include "Input/GamepadButton.h"
#include "Input/InputManager.h"
#include "Input/InputState.h"
#include "Input/KeyCode.h"
#include "Input/KeyModifierBits.h"
#include "Input/MouseButton.h"

// Node
#include "Node/BaseNode.h"
#include "Node/NodeType.h"
#include "Node/Pivot.h"
#include "Node/TransformNode.h"
#include "Node/ZOrder.h"

// Renderer
#include "Renderer/Vertex.h"
#include "Renderer/VertexData.h"

// Scene
#include "Scene/Scene.h"

// Shape
#include "Shape/AABB.h"
#include "Shape/Circle.h"
#include "Shape/OBB.h"
#include "Shape/OrientedRect.h"
#include "Shape/Plane.h"
#include "Shape/Quad.h"
#include "Shape/Quad2D.h"
#include "Shape/Rect.h"
#include "Shape/Segment.h"
#include "Shape/Segment2D.h"
#include "Shape/Sphere.h"
#include "Shape/Triangle.h"
#include "Shape/Triangle2D.h"

// Spritesheet
#include "Spritesheet/Image.h"
#include "Spritesheet/ImageType.h"

// Texture
#include "Texture/TextureManager.h"

// Utils
#include "Utils/FileSystem.h"
#include "Utils/Logger.h"
#include "Utils/Macros.h"
#include "Utils/String.h"
#include "Utils/Timer.h"
#include "Utils/Utility.h"

// Vulkan
#include "Vulkan/CommandBuffer.h"
#include "Vulkan/Context.h"
#include "Vulkan/DebugReportCallback.h"
#include "Vulkan/DebugUtilsMessenger.h"
#include "Vulkan/Devices.h"
#include "Vulkan/Instance.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/ShaderModule.h"
#include "Vulkan/SwapChain.h"
#include "Vulkan/Texture.h"
#include "Vulkan/Utils.h"

#endif