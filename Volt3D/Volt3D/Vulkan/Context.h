#ifndef CONTEXT_H
#define CONTEXT_H

#include <vulkan/vulkan.hpp>

#include <vector>
#include <optional>

#include "utils/Macros.h"

class View;
class DebugCallback;
class PhysicalDevice;
class Device;
class Surface;
class SwapChain;

namespace v3d
{
	namespace vulkan
	{
		class Context
		{
		private:
			// Context instance
			vk::UniqueInstance instance;

			// Debug + validation layer
			bool enableValidationLayer;
			DebugCallback* debugCallback;

			Surface* surface;
			PhysicalDevice* physicalDevice;
			Device* device;
			SwapChain* swapChain;

			bool initInstance(const Window& view);
			bool initDebugCallback();
			bool initSurface(const Window& view);
			bool initPhysicalDevice();
			bool initLogicalDevice();
			bool initSwapChain();
			bool initGraphicsPipeline();
			void release();

		public:
			Context();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Context);
			DEFAULT_MOVE_CONSTRUCTORS(Context);
			~Context();

			bool init(const Window& view, const bool enableValidationLayer = _DEBUG);

			std::vector<vk::ExtensionProperties> getExtensions() const;
			std::vector<vk::LayerProperties> getLayers() const;
		};
	}
}

#endif