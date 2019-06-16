#include <PreCompiled.h>

#include "GLFWCallbacks.h"

#include "Window.h"

void v3d::glfw::initCallbacks(GLFWwindow* window)
{
#ifdef V3D_GLFW_ERROR_CB
	glfwSetErrorCallback(glfwErrorCallback);
#endif
#ifdef V3D_GLFW_MONITOR_CB
	glfwSetMonitorCallback(glfwMonitorCallback);
#endif
#ifdef V3D_GLFW_WINDOW_POS_CB
	glfwSetWindowPosCallback(window, glfwWindowPosCallback);
#endif
#ifdef V3D_GLFW_WINDOW_SIZE_CB
	glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
#endif
#ifdef V3D_GLFW_WINDOW_CLOSE_CB
	glfwSetWindowCloseCallback(window, glfwWindowCloseCallback);
#endif
#ifdef V3D_GLFW_WINDOW_REFRESH_CB
	glfwSetWindowRefreshCallback(window, glfwWindowRefreshCallback);
#endif
#ifdef V3D_GLFW_WINDOW_FOCUS_CB
	glfwSetWindowFocusCallback(window, glfwWindowFocusCallback);
#endif
#ifdef V3D_GLFW_WINDOW_ICONIFY_CB
	glfwSetWindowIconifyCallback(window, glfwWindowIconifyCallback);
#endif
#ifdef V3D_GLFW_WINDOW_MAXIMIZE_CB
	glfwSetWindowMaximizeCallback(window, glfwWindowMaximizedCallback);
#endif
#ifdef V3D_GLFW_FRAMEBUFFER_SIZE_CB
	glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
#endif
#ifdef V3D_GLFW_WINDOW_CONTENT_SCALE_CB
	glfwSetWindowContentScaleCallback(window, glfwWindowContentScaleCallback);
#endif
#ifdef V3D_GLFW_KEY_CB
	glfwSetKeyCallback(window, glfwKeyCallback);
#endif
#ifdef V3D_GLFW_CHAR_CB
	glfwSetCharCallback(window, glfwCharCallback);
#endif
#ifdef V3D_GLFW_CHAR_MODS_CB
	glfwSetCharModsCallback(window, glfwCharModsCallback);
#endif
#ifdef V3D_GLFW_MOUSE_BUTTON_CB
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
#endif
#ifdef V3D_GLFW_CURSOR_POS_CB
	glfwSetCursorPosCallback(window, glfwCursorPosCallback);
#endif
#ifdef V3D_GLFW_CURSOR_ENTER_CB
	glfwSetCursorEnterCallback(window, glfwCursorEnterCallback);
#endif
#ifdef V3D_GLFW_SCROLL_CB
	glfwSetScrollCallback(window, glfwScrollCallback);
#endif
#ifdef V3D_GLFW_DROP_CB
	glfwSetDropCallback(window, glfwDropCallback);
#endif
#ifdef V3D_GLFW_JOYSTICK_CB
	glfwSetJoystickCallback(glfwJoystickCallback);
#endif
}

#ifdef V3D_GLFW_ERROR_CB
void v3d::glfw::glfwErrorCallback(int error, const char* description)
{
	v3d::Logger::getInstance().error("GLFW Error: " + std::to_string(error) + " (" + std::string(description));
}
#endif
#ifdef V3D_GLFW_MONITOR_CB
void v3d::glfw::glfwMonitorCallback(GLFWmonitor* monitor, int event)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_POS_CB
void v3d::glfw::glfwWindowPosCallback(GLFWwindow* window, int xpos, int ypos)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_SIZE_CB
void v3d::glfw::glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_CLOSE_CB
void v3d::glfw::glfwWindowCloseCallback(GLFWwindow* window)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_REFRESH_CB
void v3d::glfw::glfwWindowRefreshCallback(GLFWwindow* window)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_FOCUS_CB
void v3d::glfw::glfwWindowFocusCallback(GLFWwindow* window, int focused)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_ICONIFY_CB
void v3d::glfw::glfwWindowIconifyCallback(GLFWwindow* window, int iconified)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_MAXIMIZE_CB
void v3d::glfw::glfwWindowMaximizedCallback(GLFWwindow* window, int iconified)
{

}
#endif
#ifdef V3D_GLFW_FRAMEBUFFER_SIZE_CB
void v3d::glfw::glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{

}
#endif
#ifdef V3D_GLFW_WINDOW_CONTENT_SCALE_CB
void v3d::glfw::glfwWindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale)
{

}
#endif
#ifdef V3D_GLFW_KEY_CB
void v3d::glfw::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
		static_cast<v3d::glfw::Window*>(glfwGetWindowUserPointer(window))->closeWindow();
}
#endif
#ifdef V3D_GLFW_CHAR_CB
void v3d::glfw::glfwCharCallback(GLFWwindow* window, unsigned int codepoint)
{

}
#endif
#ifdef V3D_GLFW_CHAR_MODS_CB
void v3d::glfw::glfwCharModsCallback(GLFWwindow* window, unsigned int codepoint, int mods)
{

}
#endif
#ifdef V3D_GLFW_MOUSE_BUTTON_CB
void v3d::glfw::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}
#endif
#ifdef V3D_GLFW_CURSOR_POS_CB
void v3d::glfw::glfwCursorPosCallback(GLFWwindow* window, double x, double y)
{

}
#endif
#ifdef V3D_GLFW_CURSOR_ENTER_CB
void v3d::glfw::glfwCursorEnterCallback(GLFWwindow* window, int entered)
{

}
#endif
#ifdef V3D_GLFW_SCROLL_CB
void v3d::glfw::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{

}
#endif
#ifdef V3D_GLFW_DROP_CB
void v3d::glfw::glfwDropCallback(GLFWwindow* window, int count, const char** paths)
{

}
#endif
#ifdef V3D_GLFW_JOYSTICK_CB
void v3d::glfw::glfwJoystickCallback(int jid, int events)
{

}
#endif