/**
*	@file Vertex.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_VERTEX_H
#define V3D_VK_VERTEX_H

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include <vector>

#include "Utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		/**
		*	@class Vertex
		*	@brief Vertex
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Vertex
		{
			friend class Context;

		private:
			glm::vec3 pos;
			glm::vec3 color;

		public:
			Vertex();
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Vertex);
			DEFAULT_MOVE_CONSTRUCTORS(Vertex);
			~Vertex() {};

			static vk::VertexInputBindingDescription getInputBindingDescription();
			static std::vector<vk::VertexInputAttributeDescription> getInputAttributeDescription();
		};
	}
}

#endif