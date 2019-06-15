/**
*	@file Pipeline.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_PIPELINE_H
#define V3D_VK_PIPELINE_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class Device;
		class SwapChain;

		/**
		*	@class Pipeline
		*	@brief Wrapper of Vulkan's Pipeline
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Pipeline
		{
			friend class Context;

		private:
			vk::UniquePipeline pipeline;
			vk::Viewport viewport;
			vk::Rect2D scissor;

			bool init(const v3d::vulkan::Device& device, const v3d::vulkan::SwapChain& swapChain, const vk::RenderPass& renderPass);

		public:
			Pipeline();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Pipeline);
			DEFAULT_MOVE_CONSTRUCTORS(Pipeline);
			~Pipeline() {}

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniquePipeline, vk::Pipeline, pipeline);

			vk::Viewport getViewport() const;
			vk::Rect2D getScissor() const;
		};
	}
}

#endif