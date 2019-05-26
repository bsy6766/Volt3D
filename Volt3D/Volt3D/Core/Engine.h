#ifndef ENGINE_H
#define ENGINE_H

namespace v3d
{
	namespace glfw { class Window; }
	namespace vulkan { class Context; }

	class Engine
	{
	private:
		v3d::glfw::Window* view;
		v3d::vulkan::Context* context;

		bool loadPreference();
		bool initWindow(const std::string_view windowTitle);
		bool initContext();
		void release();

	public:
		Engine();
		~Engine();

		bool init(const std::string_view windowTitle);
		void run();

		v3d::glfw::Window& getView() const;
		v3d::vulkan::Context& getVulkanContext() const;
	};
}

#endif