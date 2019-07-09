/**
*	@file Window.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Window.h"

#include "Engine.h"
#include "Vulkan/Instance.h"
#include "Input/InputManager.h"

v3d::glfw::Window::Window(v3d::InputManager& input)
	: window(nullptr)
	, vsync(true)
	, input(input)
{}

v3d::glfw::Window::~Window() { releaseGLFW(); }

bool v3d::glfw::Window::initGLFW()
{
	auto& logger = v3d::Logger::getInstance();
	logger.info("This GLFW supports Context");

	glfwSetErrorCallback(v3d::glfw::Window::glfwErrorCallback);

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

bool v3d::glfw::Window::initWindow(const char* windowTitle)
{
	initGLFWHints();

	setVsync(vsync);

	window = glfwCreateWindow(1280, 720, windowTitle, nullptr, nullptr);
	if (!window) return false;

	//glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	
	initCallbacks(window);

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

void v3d::glfw::Window::setVsync(const bool enabled)
{
	vsync = enabled;
	glfwSwapInterval(vsync ? 1 : 0);
}

bool v3d::glfw::Window::isVsyncEnabled() const
{
	return vsync;
}

bool v3d::glfw::Window::isIconified() const
{
	return glfwGetWindowAttrib(window, GLFW_ICONIFIED);
}

glm::uvec2 v3d::glfw::Window::getFrameBufferSize() const
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	return glm::uvec2(w, h);
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







void v3d::glfw::Window::initCallbacks(GLFWwindow* window)
{
#ifdef V3D_GLFW_ERROR_CB
	glfwSetErrorCallback(v3d::glfw::Window::glfwErrorCallback);
#endif
#ifdef V3D_GLFW_MONITOR_CB
	glfwSetMonitorCallback(v3d::glfw::Window::glfwMonitorCallback);
#endif
#ifdef V3D_GLFW_WINDOW_POS_CB
	glfwSetWindowPosCallback(window, v3d::glfw::Window::glfwWindowPosCallback);
#endif
#ifdef V3D_GLFW_WINDOW_SIZE_CB
	glfwSetWindowSizeCallback(window, v3d::glfw::Window::glfwWindowSizeCallback);
#endif
#ifdef V3D_GLFW_WINDOW_CLOSE_CB
	glfwSetWindowCloseCallback(window, v3d::glfw::Window::glfwWindowCloseCallback);
#endif
#ifdef V3D_GLFW_WINDOW_REFRESH_CB
	glfwSetWindowRefreshCallback(window, v3d::glfw::Window::glfwWindowRefreshCallback);
#endif
#ifdef V3D_GLFW_WINDOW_FOCUS_CB
	glfwSetWindowFocusCallback(window, v3d::glfw::Window::glfwWindowFocusCallback);
#endif
#ifdef V3D_GLFW_WINDOW_ICONIFY_CB
	glfwSetWindowIconifyCallback(window, v3d::glfw::Window::glfwWindowIconifyCallback);
#endif
#ifdef V3D_GLFW_WINDOW_MAXIMIZE_CB
	glfwSetWindowMaximizeCallback(window, v3d::glfw::Window::glfwWindowMaximizedCallback);
#endif
#ifdef V3D_GLFW_FRAMEBUFFER_SIZE_CB
	glfwSetFramebufferSizeCallback(window, v3d::glfw::Window::glfwFramebufferSizeCallback);
#endif
#ifdef V3D_GLFW_WINDOW_CONTENT_SCALE_CB
	glfwSetWindowContentScaleCallback(window, v3d::glfw::Window::glfwWindowContentScaleCallback);
#endif
#ifdef V3D_GLFW_KEY_CB
	glfwSetKeyCallback(window, v3d::glfw::Window::glfwKeyCallback);
#endif
#ifdef V3D_GLFW_CHAR_CB
	glfwSetCharCallback(window, v3d::glfw::Window::glfwCharCallback);
#endif
#ifdef V3D_GLFW_CHAR_MODS_CB
	glfwSetCharModsCallback(window, v3d::glfw::Window::glfwCharModsCallback);
#endif
#ifdef V3D_GLFW_MOUSE_BUTTON_CB
	glfwSetMouseButtonCallback(window, v3d::glfw::Window::glfwMouseButtonCallback);
#endif
#ifdef V3D_GLFW_CURSOR_POS_CB
	glfwSetCursorPosCallback(window, v3d::glfw::Window::glfwCursorPosCallback);
#endif
#ifdef V3D_GLFW_CURSOR_ENTER_CB
	glfwSetCursorEnterCallback(window, v3d::glfw::Window::glfwCursorEnterCallback);
#endif
#ifdef V3D_GLFW_SCROLL_CB
	glfwSetScrollCallback(window, v3d::glfw::Window::glfwScrollCallback);
#endif
#ifdef V3D_GLFW_DROP_CB
	glfwSetDropCallback(window, v3d::glfw::Window::glfwDropCallback);
#endif
#ifdef V3D_GLFW_JOYSTICK_CB
	glfwSetJoystickCallback(v3d::glfw::Window::glfwJoystickCallback);
#endif
}

#ifdef V3D_GLFW_ERROR_CB
void v3d::glfw::Window::glfwErrorCallback(int error, const char* description)
{
	v3d::Logger::getInstance().error("GLFW Error: " + std::to_string(error) + " (" + std::string(description));
}
#endif
#ifdef V3D_GLFW_MONITOR_CB
void v3d::glfw::Window::glfwMonitorCallback(GLFWmonitor* monitor, int event)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_POS_CB
void v3d::glfw::Window::glfwWindowPosCallback(GLFWwindow* window, int xpos, int ypos)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_SIZE_CB
void v3d::glfw::Window::glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_CLOSE_CB
void v3d::glfw::Window::glfwWindowCloseCallback(GLFWwindow* window)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_REFRESH_CB
void v3d::glfw::Window::glfwWindowRefreshCallback(GLFWwindow* window)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_FOCUS_CB
void v3d::glfw::Window::glfwWindowFocusCallback(GLFWwindow* window, int focused)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_ICONIFY_CB
void v3d::glfw::Window::glfwWindowIconifyCallback(GLFWwindow* window, int iconified)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_MAXIMIZE_CB
void v3d::glfw::Window::glfwWindowMaximizedCallback(GLFWwindow* window, int iconified)
{

}
#endif
#ifdef V3D_GLFW_FRAMEBUFFER_SIZE_CB
void v3d::glfw::Window::glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_CONTENT_SCALE_CB
void v3d::glfw::Window::glfwWindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale)
{

}
#endif
#ifdef V3D_GLFW_KEY_CB
void v3d::glfw::Window::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		static_cast<v3d::glfw::Window*>(glfwGetWindowUserPointer(window))->closeWindow();
}
#endif
#ifdef V3D_GLFW_CHAR_CB
void v3d::glfw::Window::glfwCharCallback(GLFWwindow* window, unsigned int codepoint)
{

}
#endif
#ifdef V3D_GLFW_CHAR_MODS_CB
void v3d::glfw::Window::glfwCharModsCallback(GLFWwindow* window, unsigned int codepoint, int mods)
{

}
#endif
#ifdef V3D_GLFW_MOUSE_BUTTON_CB
void v3d::glfw::Window::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}
#endif
#ifdef V3D_GLFW_CURSOR_POS_CB
void v3d::glfw::Window::glfwCursorPosCallback(GLFWwindow* window, double x, double y)
{
	static_cast<v3d::glfw::Window*>(glfwGetWindowUserPointer(window))->onCursorPos(int(x), int(y));
}
#endif
#ifdef V3D_GLFW_CURSOR_ENTER_CB
void v3d::glfw::Window::glfwCursorEnterCallback(GLFWwindow* window, int entered)
{

}
#endif
#ifdef V3D_GLFW_SCROLL_CB
void v3d::glfw::Window::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}
#endif
#ifdef V3D_GLFW_DROP_CB
void v3d::glfw::Window::glfwDropCallback(GLFWwindow* window, int count, const char** paths)
{

}
#endif
#ifdef V3D_GLFW_JOYSTICK_CB
void v3d::glfw::Window::glfwJoystickCallback(int jid, int events)
{

}
#endif



void v3d::glfw::Window::onCursorPos(const int x, const int y)
{
	input.updateMousePosition(x, y);
}