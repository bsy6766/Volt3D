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
	class VOLT3D_DLL V3_C4
	{
	public:
		glm::vec3 vertex;
		glm::vec4 color;

		V3_C4();
		V3_C4( const glm::vec3& vertex, const glm::vec4& color );
		~V3_C4() {};

		static vk::VertexInputBindingDescription getInputBindingDescription();
		static std::vector<vk::VertexInputAttributeDescription> getInputAttributeDescription();
	};

	class VOLT3D_DLL V3_T2
	{
	public:
		glm::vec3 vertex;
		glm::vec2 texCoord;

		V3_T2();
		V3_T2( const glm::vec3& vertex, const glm::vec2& texCoord );
		~V3_T2() {};

		static vk::VertexInputBindingDescription getInputBindingDescription();
		static std::vector<vk::VertexInputAttributeDescription> getInputAttributeDescription();
	};

	class VOLT3D_DLL V3_C4_T2
	{
	public:
		glm::vec3 vertex;
		glm::vec4 color;
		glm::vec2 texCoord;

		V3_C4_T2();
		V3_C4_T2( const glm::vec3& vertex, const glm::vec4& color, const glm::vec2& texCoord );
		~V3_C4_T2() {};

		static vk::VertexInputBindingDescription getInputBindingDescription();
		static std::vector<vk::VertexInputAttributeDescription> getInputAttributeDescription();
	};
}

#endif