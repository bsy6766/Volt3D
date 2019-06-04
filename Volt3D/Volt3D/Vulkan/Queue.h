/**
*	@file Queue.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_QUEUE_H
#define V3D_VK_QUEUE_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class PhysicalDevice;
		class Device;

		/**
		*	@class Queue
		*	@brief A wrapper for Vulkan's queue.
		*
		*	@group vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Queue
		{
			friend class Context;

		private:
			Queue();

			vk::Queue graphicsQueue;
			vk::Queue presentQueue;

			bool init(const v3d::vulkan::PhysicalDevice& physicalDevice, const v3d::vulkan::Device& device);

		public:
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Queue);
			DEFAULT_MOVE_CONSTRUCTORS(Queue);
			~Queue() {}

			const vk::Queue& getGraphicsQueue() const;
			const vk::Queue& getPresentQueue() const;

			void submit(const vk::SubmitInfo& submitInfo) const;
			void present(const vk::PresentInfoKHR& presentInfo) const;
		};
	}
}

#endif