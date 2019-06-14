/**
*	@file Surface.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Surface.h"

#include "Core/Window.h"
#include "Instance.h"

v3d::vulkan::Surface::Surface()
	: surface()
{}

bool v3d::vulkan::Surface::init(const v3d::glfw::Window& window, const v3d::vulkan::Instance& instance)
{
	VkSurfaceKHR cVkSurfaceKHR;
	if (!window.createWindowSurface(instance, cVkSurfaceKHR)) return false;
	surface = std::move(vk::UniqueSurfaceKHR(cVkSurfaceKHR, instance.get()));
	return true;
}
