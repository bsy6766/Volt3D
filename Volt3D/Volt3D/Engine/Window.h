/**
*	@file Window.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_GLFW_WINDOW_H
#define V3D_GLFW_WINDOW_H

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include <vector>

#include "Config/GLFWCallbackConfig.h"

#include "utils/Macros.h"
#include "WindowMode.h"
#include "CursorMode.h"

struct GLFWwindow;

namespace v3d
{
	class InputManager;
	namespace vulkan { class Instance; class Context; }
	namespace glfw
	{
		/**
		*	@class Window
		*	@brief Simple wrapper for GLFW
		*
		*	@group GLFW
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Window
		{
			friend class Engine;
			friend class vulkan::Instance;
			friend class vulkan::Context;

		private:
			Window() = delete;
			Window(v3d::InputManager& input);

			GLFWwindow* window;
			InputManager& input;

			bool vsync;

			v3d::WindowMode windowMode;
			v3d::CursorMode cursorMode;

			// GLFW
			bool initGLFW();
			bool initWindow(const char* windowTitle, const int width, const int height, const v3d::WindowMode windowMode);
			void initGLFWHints();
			void releaseGLFW();

			// VK
			std::size_t getGLFWVKExtensions(std::vector<const char*>& extensions) const;
			bool createWindowSurface(const v3d::vulkan::Instance& instance, VkSurfaceKHR& surface) const;

			// listener from GLFW callback
			void onCursorPos(const int x, const int y);

			void initCallbacks(GLFWwindow* window);

#ifdef V3D_GLFW_ERROR_CB
			static void glfwErrorCallback(int error, const char* description);
#endif
#ifdef V3D_GLFW_MONITOR_CB
			static void glfwMonitorCallback(GLFWmonitor* monitor, int event);
#endif
#ifdef V3D_GLFW_WINDOW_POS_CB
			static void glfwWindowPosCallback(GLFWwindow* window, int xpos, int ypos);
#endif
#ifdef V3D_GLFW_WINDOW_SIZE_CB
			static void glfwWindowSizeCallback(GLFWwindow* window, int width, int height);
#endif
#ifdef V3D_GLFW_WINDOW_CLOSE_CB
			static void glfwWindowCloseCallback(GLFWwindow* window);
#endif
#ifdef V3D_GLFW_WINDOW_REFRESH_CB
			static void glfwWindowRefreshCallback(GLFWwindow* window);
#endif
#ifdef V3D_GLFW_WINDOW_FOCUS_CB
			static void glfwWindowFocusCallback(GLFWwindow* window, int focused);
#endif
#ifdef V3D_GLFW_WINDOW_ICONIFY_CB
			static void glfwWindowIconifyCallback(GLFWwindow* window, int iconified);
#endif
#ifdef V3D_GLFW_WINDOW_MAXIMIZE_CB
			static void glfwWindowMaximizedCallback(GLFWwindow* window, int iconified);
#endif
#ifdef V3D_GLFW_FRAMEBUFFER_SIZE_CB
			static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
#endif
#ifdef V3D_GLFW_WINDOW_CONTENT_SCALE_CB
			static void glfwWindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale);
#endif
#ifdef V3D_GLFW_KEY_CB
			static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
#endif
#ifdef V3D_GLFW_CHAR_CB
			static void glfwCharCallback(GLFWwindow* window, unsigned int codepoint);
#endif
#ifdef V3D_GLFW_CHAR_MODS_CB
			static void glfwCharModsCallback(GLFWwindow* window, unsigned int codepoint, int mods);
#endif
#ifdef V3D_GLFW_MOUSE_BUTTON_CB
			static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
#endif
#ifdef V3D_GLFW_CURSOR_POS_CB
			static void glfwCursorPosCallback(GLFWwindow* window, double x, double y);
#endif
#ifdef V3D_GLFW_CURSOR_ENTER_CB
			static void glfwCursorEnterCallback(GLFWwindow* window, int entered);
#endif
#ifdef V3D_GLFW_SCROLL_CB
			static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
#endif
#ifdef V3D_GLFW_DROP_CB
			static void glfwDropCallback(GLFWwindow* window, int count, const char** paths);
#endif
#ifdef V3D_GLFW_JOYSTICK_CB
			static void glfwJoystickCallback(int jid, int events);
#endif

		public:
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Window);
			DEFAULT_MOVE_CONSTRUCTORS(Window);
			~Window();

			/**
			*	Check if window is running.
			*	@return true if window is running. Else, false.
			*/
			bool isRunning();
			void pollGLFWEvent();

			/**
			*	Close window.
			*	This will terminate the application after current loop is finished.
			*	@return true if successfully attempted to close the window. Else, false.
			*/
			bool closeWindow();

			void setVsync(const bool enabled);

			/**
			*	Check if vsync is enabled.
			*	@return true if vsync is enabled. Else, false.
			*/
			bool isVsyncEnabled() const;

			/**
			*	Check if window is iconified (minimized)
			*	@return true if window is iconified. Else, false.
			*/
			bool isIconified() const;

			/**
			*	Get frame buffer size.
			*	@return A frame buffer size.
			*/
			glm::uvec2 getFrameBufferSize() const;

			/**
			*	Get window size.
			*	@return A current window size.
			*/
			glm::uvec2 getWindowSize() const;

			/**
			*	Get window title.
			*	@todo Support this
			*	@note This doesn't use GLFW's function due to lack of support. Uses Win32 API.
			*	@return A title of window in UTF8 char array.
			*/
			//const char* getWindowTitle() const;

			/**
			*	Set window title.
			*	@see GLFW's window documentation for detaisl.
			*	@param newTitle A new window title to set.
			*/
			void setWindowTitle( const char* newTitle );

			/**
			*	Set cursor mode.
			*	@see v3d::CursorMode for details.
			*	@param newCursorMode A new cursor mode to set.
			*/
			void setCursorMode( const v3d::CursorMode newCursorMode ) { cursorMode = newCursorMode; }
			
			/**
			*	Get cursor mode.
			*	@see v3d::CursorMode for details.
			*	@return Current cursor mode of this window.
			*/
			v3d::CursorMode getCursorMode() const { return cursorMode; }
		};
	}
}

#endif