/**
*	@file Vertex.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Vertex.h"

v3d::vulkan::Vertex::Vertex()
	: pos(0)
	, color(0)
{}

v3d::vulkan::Vertex::Vertex(const glm::vec3 & pos, const glm::vec3 & color)
	: pos(pos)
	, color(color)
{}

vk::VertexInputBindingDescription v3d::vulkan::Vertex::getInputBindingDescription()
{
	return vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);
}

std::vector<vk::VertexInputAttributeDescription> v3d::vulkan::Vertex::getInputAttributeDescription()
{
	return { vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, pos)), vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color)) };
}
