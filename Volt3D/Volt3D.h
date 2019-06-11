/**
*	@file Volt3D.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef VOLT3D_H
#define VOLT3D_H

// Color
#include "Color/Color3B.h"
#include "Color/Color3F.h"
#include "Color/Color4B.h"
#include "Color/Color4F.h"

// Config
#include "Config/BuildConfig.h"
#include "Config/GLFWCallbackConfig.h"

// Core
#include "Core/Engine.h"
#include "Core/GLFWCallbacks.h"
#include "Core/Preference.h"
#include "Core/Time.h"
#include "Core/Window.h"

// Enums
#include "Enums/WindowMode.h"

// Renderer
#include "Renderer/VertexData.h"

// Utils
#include "Utils/FileSystem.h"
#include "Utils/Logger.h"
#include "Utils/Macros.h"
#include "Utils/Timer.h"

// Vulkan
#include "Vulkan/Buffer.h"
#include "Vulkan/CommandBuffer.h"
#include "Vulkan/CommandPool.h"
#include "Vulkan/Context.h"
#include "Vulkan/DebugReportCallback.h"
#include "Vulkan/DebugUtilsMessenger.h"
#include "Vulkan/Device.h"
#include "Vulkan/Fence.h"
#include "Vulkan/FrameBuffer.h"
#include "Vulkan/Instance.h"
#include "Vulkan/PhysicalDevice.h"
#include "Vulkan/Pipeline.h"
#include "Vulkan/Queue.h"
#include "Vulkan/RenderPass.h"
#include "Vulkan/Semaphore.h"
#include "Vulkan/ShaderModule.h"
#include "Vulkan/Surface.h"
#include "Vulkan/SwapChain.h"
#include "Vulkan/Utils.h"
#include "Vulkan/Vertex.h"

#endif