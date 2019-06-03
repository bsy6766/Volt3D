/**
*	@file Context.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_CONTEXT_H
#define V3D_VK_CONTEXT_H

#include <vulkan/vulkan.hpp>

#include <vector>

#include "utils/Macros.h"
#include "Config/BuildConfig.h"

namespace v3d
{
	namespace glfw { class Window; }
	namespace vulkan
	{
		class View;
		class Instance;
		class DebugReportCallback;
		class DebugUtilsMessenger;
		class PhysicalDevice;
		class Device;
		class Surface;
		class SwapChain;
		class RenderPass;
		class Pipeline;
		class FrameBuffer;

		/**
		*	@class Context
		*	@brief Vulkan context.
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Context
		{
		private:
			// Context instance
			v3d::vulkan::Instance* instance;

			bool validationLayerEnabled;
			v3d::vulkan::DebugReportCallback* debugReportCallback;
			v3d::vulkan::DebugUtilsMessenger* debugUtilsMessenger;
			v3d::vulkan::Surface* surface;
			v3d::vulkan::PhysicalDevice* physicalDevice;
			v3d::vulkan::Device* device;
			v3d::vulkan::SwapChain* swapChain;
			v3d::vulkan::RenderPass* renderPass;
			v3d::vulkan::Pipeline* pipeline;
			v3d::vulkan::FrameBuffer* frameBuffer;

			bool initInstance(const v3d::glfw::Window& view);
			bool initDebugReport();
			bool initDebugUtilsMessenger();
			bool initSurface(const v3d::glfw::Window& view);
			bool initPhysicalDevice();
			bool initDevice();
			bool initSwapChain();
			bool initRenderPass();
			bool initGraphicsPipeline();
			bool initFrameBuffer();
			void release();

		public:
			Context();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Context);
			DEFAULT_MOVE_CONSTRUCTORS(Context);
			~Context();

			bool init(const v3d::glfw::Window& window, const bool enableValidationLayer);
		};
	}
}

#endif