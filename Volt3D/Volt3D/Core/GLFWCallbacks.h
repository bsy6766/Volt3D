#ifndef GLFW_CALLBACKS_H
#define GLFW_CALLBACKS_H

#include "Config/GLFWCallbackConfig.h"

namespace v3d
{
	namespace glfw
	{
		void initCallbacks(GLFWwindow* window);

#ifdef V3D_GLFW_ERROR_CB
		void glfwErrorCallback(int error, const char* description);
#endif
#ifdef V3D_GLFW_MONITOR_CB
		void glfwMonitorCallback(GLFWmonitor* monitor, int event);
#endif
#ifdef V3D_GLFW_WINDOW_POS_CB
		void glfwWindowPosCallback(GLFWwindow* window, int xpos, int ypos);
#endif
#ifdef V3D_GLFW_WINDOW_SIZE_CB
		void glfwWindowSizeCallback(GLFWwindow* window, int width, int height);
#endif
#ifdef V3D_GLFW_WINDOW_CLOSE_CB
		void glfwWindowCloseCallback(GLFWwindow* window);
#endif
#ifdef V3D_GLFW_WINDOW_REFRESH_CB
		void glfwWindowRefreshCallback(GLFWwindow* window);
#endif
#ifdef V3D_GLFW_WINDOW_FOCUS_CB
		void glfwWindowFocusCallback(GLFWwindow* window, int focused);
#endif
#ifdef V3D_GLFW_WINDOW_ICONIFY_CB
		void glfwWindowIconifyCallback(GLFWwindow* window, int iconified);
#endif
#ifdef V3D_GLFW_WINDOW_MAXIMIZE_CB
		void glfwWindowMaximizedCallback(GLFWwindow* window, int iconified);
#endif
#ifdef V3D_GLFW_FRAMEBUFFER_SIZE_CB
		void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
#endif
#ifdef V3D_GLFW_WINDOW_CONTENT_SCALE_CB
		void glfwWindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale);
#endif
#ifdef V3D_GLFW_KEY_CB
		void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
#endif
#ifdef V3D_GLFW_CHAR_CB
		void glfwCharCallback(GLFWwindow* window, unsigned int codepoint);
#endif
#ifdef V3D_GLFW_CHAR_MODS_CB
		void glfwCharModsCallback(GLFWwindow* window, unsigned int codepoint, int mods);
#endif
#ifdef V3D_GLFW_MOUSE_BUTTON_CB
		void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
#endif
#ifdef V3D_GLFW_CURSOR_POS_CB
		void glfwCursorPosCallback(GLFWwindow* window, double x, double y);
#endif
#ifdef V3D_GLFW_CURSOR_ENTER_CB
		void glfwCursorEnterCallback(GLFWwindow* window, int entered);
#endif
#ifdef V3D_GLFW_SCROLL_CB
		void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
#endif
#ifdef V3D_GLFW_DROP_CB
		void glfwDropCallback(GLFWwindow* window, int count, const char** paths);
#endif
#ifdef V3D_GLFW_JOYSTICK_CB
		void glfwJoystickCallback(int jid, int events);
#endif
	}
}
#endif