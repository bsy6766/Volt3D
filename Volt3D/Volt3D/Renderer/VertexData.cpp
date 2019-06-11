/**
*	@file VertexData.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "VertexData.h"

v3d::VertexData::VertexData()
	: vertexData()
{}

std::size_t v3d::VertexData::getDataSize() const
{
	return sizeof(v3d::vulkan::Vertex) * vertexData.size();
}
