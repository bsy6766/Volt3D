/**
*	@file Window.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Window.h"

#include "GLFWCallbacks.h"
#include "Vulkan/Instance.h"

v3d::glfw::Window::Window()
	: window(nullptr)
{}

v3d::glfw::Window::~Window() { releaseGLFW(); }

bool v3d::glfw::Window::initGLFW()
{
	auto& logger = v3d::Logger::getInstance();
	logger.info("This GLFW supports Context");

	glfwSetErrorCallback(v3d::glfw::glfwErrorCallback);

	if (glfwInit() != GL_TRUE)
	{
		return false;
	}
	else
	{
		if (glfwVulkanSupported() == GLFW_FALSE) { logger.critical("Context is not supported with this GLFW"); glfwTerminate(); return false; }

		int versionMajor, versionMinor, versionRev;
		glfwGetVersion(&versionMajor, &versionMinor, &versionRev);

		logger.info("GLFW verison: " + std::to_string(versionMajor) + "." + std::to_string(versionMinor) + "." + std::to_string(versionRev));

		return true;
	}
}

bool v3d::glfw::Window::initWindow(const std::string_view windowTitle)
{
	initGLFWHints();

	window = glfwCreateWindow(1280, 720, std::string(windowTitle).c_str(), nullptr, nullptr);
	if (!window) return false;

	//glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	
	v3d::glfw::initCallbacks(window);

	return true;
}

void v3d::glfw::Window::initGLFWHints()
{
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

bool v3d::glfw::Window::createWindowSurface(const v3d::vulkan::Instance& instance, VkSurfaceKHR & surface) const
{
	if (glfwCreateWindowSurface(instance.get(), window, nullptr, &surface) != VK_SUCCESS)
	{
		v3d::Logger::getInstance().critical("Failed to create KHR Surface");
		return false;
	}

	return true;
}

bool v3d::glfw::Window::isRunning()
{
	if (window) return !glfwWindowShouldClose(window); else return false;
}

void v3d::glfw::Window::pollGLFWEvent()
{
	glfwPollEvents();
}

bool v3d::glfw::Window::closeWindow()
{
	if (window) { glfwSetWindowShouldClose(window, GLFW_TRUE); return true; }
	else return false;
}

void v3d::glfw::Window::releaseGLFW()
{
	closeWindow();
	if (window) { glfwDestroyWindow(window); window = nullptr; }
	glfwTerminate();
}

std::size_t v3d::glfw::Window::getGLFWVKExtensions(std::vector<const char*> & extensions) const
{
	extensions.clear();

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	extensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);

	return glfwExtensionCount;
}