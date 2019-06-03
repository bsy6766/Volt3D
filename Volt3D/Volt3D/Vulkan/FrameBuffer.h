/**
*	@file FrameBuffer.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_FRAME_BUFFER_H
#define V3D_VK_FRAME_BUFFER_H

#include <vulkan/vulkan.hpp>

#include <vector>

#include "Utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class SwapChain;
		class RenderPass;
		class Device;

		/**
		*	@class FrameBuffer
		*	@brief A wrapper for Vulkan's framebuffer.
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL FrameBuffer
		{
			friend class Context;

		private:
			std::vector<vk::UniqueFramebuffer> frameBuffers;

			bool init(const v3d::vulkan::SwapChain& swapChain, const v3d::vulkan::RenderPass& renderPass, const v3d::vulkan::Device& device);

		public:
			FrameBuffer();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(FrameBuffer);
			DEFAULT_MOVE_CONSTRUCTORS(FrameBuffer);
			~FrameBuffer() {}
		};
	}
}

#endif