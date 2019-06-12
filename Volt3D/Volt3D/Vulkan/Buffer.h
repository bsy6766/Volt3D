/**
*	@file Buffer.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_BUFFER_H
#define V3D_BUFFER_H

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include <vector>

#include "Utils/Macros.h"

namespace v3d
{
	class VertexData;

	namespace vulkan
	{
		class Device;

		/**
		*	@class Buffer
		*	@brife A wrapper for Vulkan's buffer
		*/
		class VOLT3D_DLL Buffer
		{
			friend class Context;

		private:
			vk::UniqueBuffer buffer;

			bool init(const v3d::vulkan::Device& device, const v3d::VertexData& vertexData, const vk::BufferUsageFlags usageFlag);

		public:
			Buffer();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Buffer);
			DEFAULT_MOVE_CONSTRUCTORS(Buffer);
			~Buffer() {};

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueBuffer, vk::Buffer, buffer);
		};
	}
}

#endif