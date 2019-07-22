/**
*	@file Vertex.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Vertex.h"

//v3d::Vertex::Vertex()
//	: pos(0)
//	, color(0)
//{}
//
//v3d::Vertex::Vertex(const glm::vec3 & pos, const glm::vec3 & color)
//	: pos(pos)
//	, color(color)
//{}
//
//vk::VertexInputBindingDescription v3d::Vertex::getInputBindingDescription()
//{
//	return vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);
//}
//
//std::vector<vk::VertexInputAttributeDescription> v3d::Vertex::getInputAttributeDescription()
//{
//	return 
//	{ 
//		vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, pos)), 
//		vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color)) 
//	};
//}

v3d::V3_C4::V3_C4()
	: vertex(0)
	, color(0)
{}

v3d::V3_C4::V3_C4( const glm::vec3 & vertex, const glm::vec4 & color )
	: vertex(vertex)
	, color(color)
{}

vk::VertexInputBindingDescription v3d::V3_C4::getInputBindingDescription()
{
	return vk::VertexInputBindingDescription( 0, sizeof( v3d::V3_C4 ), vk::VertexInputRate::eVertex );
}

std::vector<vk::VertexInputAttributeDescription> v3d::V3_C4::getInputAttributeDescription()
{
	return 
	{ 
		vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof( v3d::V3_C4, vertex)),
		vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32A32Sfloat, offsetof( v3d::V3_C4, color))
	};
}




v3d::V3_T2::V3_T2()
	: vertex( 0 )
	, texCoord( 0 )
{}

v3d::V3_T2::V3_T2( const glm::vec3& vertex, const glm::vec2& texCoord )
	: vertex( vertex )
	, texCoord( texCoord )
{}

vk::VertexInputBindingDescription v3d::V3_T2::getInputBindingDescription()
{
	return vk::VertexInputBindingDescription( 0, sizeof( v3d::V3_T2 ), vk::VertexInputRate::eVertex );
}

std::vector<vk::VertexInputAttributeDescription> v3d::V3_T2::getInputAttributeDescription()
{
	return
	{
		vk::VertexInputAttributeDescription( 0, 0, vk::Format::eR32G32B32Sfloat, offsetof( v3d::V3_T2, vertex ) ),
		vk::VertexInputAttributeDescription( 1, 0, vk::Format::eR32G32Sfloat, offsetof( v3d::V3_T2, texCoord ) )
	};
}

