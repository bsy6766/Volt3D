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

vk::VertexInputBindingDescription v3d::vulkan::Vertex::getInputBindingDescription()
{
	return vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);
}

std::vector<vk::VertexInputAttributeDescription> v3d::vulkan::Vertex::getInputAttributeDescription()
{
	return { vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, pos)), vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color)) };
}
