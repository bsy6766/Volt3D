/**
*	@file VertexData.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "VertexData.h"

v3d::VertexData::VertexData()
	: vertices()
{}

std::size_t v3d::VertexData::getSize() const
{
	return vertices.size();
}

std::size_t v3d::VertexData::getDataSize() const
{
	return sizeof(v3d::vulkan::Vertex) * vertices.size();
}

std::vector<v3d::vulkan::Vertex>& v3d::VertexData::getVertexData()
{
	return vertices;
}

const v3d::vulkan::Vertex* v3d::VertexData::getData() const
{
	return vertices.data();
}
