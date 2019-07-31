﻿/**
*	@file Pipeline.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Pipeline.h"

#include "ShaderModule.h"
#include "Vulkan/SwapChain.h"
#include "Renderer/Vertex.h"

v3d::vulkan::Pipeline::Pipeline()
	: pipeline()
	, viewport()
	, scissor()
{}

const vk::Viewport& v3d::vulkan::Pipeline::getViewport() const
{
	return viewport;
}

const vk::Rect2D& v3d::vulkan::Pipeline::getScissor() const
{
	return scissor;
}

const vk::UniquePipelineLayout& v3d::vulkan::Pipeline::getLayout() const
{
	return pipelineLayout;
}

bool v3d::vulkan::Pipeline::init( const vk::Device& logicalDevice, const v3d::vulkan::SwapChain& swapChain, const vk::RenderPass& renderPass, const vk::DescriptorSetLayout& descriptorSetLayout)
{
	vk::PipelineLayoutCreateInfo layoutCreateInfo
	(
		vk::PipelineLayoutCreateFlags(),
		1, &descriptorSetLayout,
		0, nullptr
	);

	pipelineLayout = logicalDevice.createPipelineLayoutUnique(layoutCreateInfo);

	v3d::vulkan::ShaderModule vertShader;
	if (!vertShader.init("Shaders/vert.spv", logicalDevice )) return false;

	v3d::vulkan::ShaderModule fragShader;
	if (!fragShader.init("Shaders/frag.spv", logicalDevice )) return false;

	shaderCreateInfos.push_back( vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertShader.get(), "main" ) );
	shaderCreateInfos.push_back( vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragShader.get(), "main" ) );
	
	vertexInputAttribDescriptions = v3d::V3_C4_T2::getInputAttributeDescription();
	vertexInputBindingDescription = v3d::V3_C4_T2::getInputBindingDescription();
	vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo
	(
		vk::PipelineVertexInputStateCreateFlags(),
		1, &vertexInputBindingDescription,
		3, vertexInputAttribDescriptions.data()
	);

	vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo
	(
		vk::PipelineInputAssemblyStateCreateFlags(),
		vk::PrimitiveTopology::eTriangleList
	);

	const vk::Extent2D& extent = swapChain.getExtent2D();

	viewport = vk::Viewport
	(
		0.0f, 0.0f,
		static_cast<float>(extent.width),
		static_cast<float>(extent.height),
		0.0f, 1.0f
	);

	scissor = vk::Rect2D(vk::Offset2D(0, 0), extent);

	vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo
	(
		vk::PipelineViewportStateCreateFlags(),
		1, &viewport,
		1, &scissor
	);

	vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo
	(
		vk::PipelineRasterizationStateCreateFlags(),  // flags
		false,                                        // depthClampEnable
		false,                                        // rasterizerDiscardEnable
		vk::PolygonMode::eFill,                       // polygonMode
		vk::CullModeFlagBits::eBack,                  // cullMode
		vk::FrontFace::eClockwise,                    // frontFace
		false,                                        // depthBiasEnable
		0.0f,                                         // depthBiasConstantFactor
		0.0f,                                         // depthBiasClamp
		0.0f,                                         // depthBiasSlopeFactor
		1.0f                                          // lineWidth
	);

	// Not using multisampling
	vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;

	// @note visit later
	// Depth and stencil
	//vk::StencilOpState stencilOpState(vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::StencilOp::eKeep, vk::CompareOp::eAlways);
	//vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo
	//(
	//	vk::PipelineDepthStencilStateCreateFlags(), // flags
	//	true,                                       // depthTestEnable
	//	true,                                       // depthWriteEnable
	//	vk::CompareOp::eLessOrEqual,                // depthCompareOp
	//	false,                                      // depthBoundTestEnable
	//	false,                                      // stencilTestEnable
	//	stencilOpState,                             // front
	//	stencilOpState                              // back
	//);

	vk::ColorComponentFlags colorComponentFlags
	(
		vk::ColorComponentFlagBits::eR |
		vk::ColorComponentFlagBits::eG |
		vk::ColorComponentFlagBits::eB |
		vk::ColorComponentFlagBits::eA
	);

	// Blending off
	//vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState
	//(
	//	false,                      // blendEnable
	//	vk::BlendFactor::eZero,     // srcColorBlendFactor
	//	vk::BlendFactor::eZero,     // dstColorBlendFactor
	//	vk::BlendOp::eAdd,          // colorBlendOp
	//	vk::BlendFactor::eZero,     // srcAlphaBlendFactor
	//	vk::BlendFactor::eZero,     // dstAlphaBlendFactor
	//	vk::BlendOp::eAdd,          // alphaBlendOp
	//	colorComponentFlags         // colorWriteMask
	//);

	// typical blending
	vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState
	(
		true,									// blendEnable
		vk::BlendFactor::eSrcAlpha,				// srcColorBlendFactor
		vk::BlendFactor::eOneMinusSrcAlpha,     // dstColorBlendFactor
		vk::BlendOp::eAdd,						// colorBlendOp
		vk::BlendFactor::eOne,					// srcAlphaBlendFactor
		vk::BlendFactor::eZero,					// dstAlphaBlendFactor
		vk::BlendOp::eAdd,						// alphaBlendOp
		colorComponentFlags						// colorWriteMask
	);

	vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo
	(
		vk::PipelineColorBlendStateCreateFlags(),   // flags
		false,                                      // logicOpEnable
		vk::LogicOp::eNoOp,                         // logicOp
		1,                                          // attachmentCount
		&pipelineColorBlendAttachmentState,         // pAttachments
		{ { (1.0f, 1.0f, 1.0f, 1.0f) } }            // blendConstants
	);

	vk::DynamicState dynamicStates[2] =
	{
		vk::DynamicState::eViewport,
		vk::DynamicState::eScissor
	};

	vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo
	(
		vk::PipelineDynamicStateCreateFlags(),
		2, dynamicStates
	);

	vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo
	(
		vk::PipelineCreateFlags(),                  // flags
		uint32_t(shaderCreateInfos.size()),			// stageCount
		shaderCreateInfos.data(),					// pStages
		&pipelineVertexInputStateCreateInfo,        // pVertexInputState
		&pipelineInputAssemblyStateCreateInfo,      // pInputAssemblyState
		nullptr,                                    // pTessellationState
		&pipelineViewportStateCreateInfo,           // pViewportState
		&pipelineRasterizationStateCreateInfo,      // pRasterizationState
		&pipelineMultisampleStateCreateInfo,        // pMultisampleState
		nullptr,							        // pDepthStencilState
		&pipelineColorBlendStateCreateInfo,         // pColorBlendState
		&pipelineDynamicStateCreateInfo,            // pDynamicState
		pipelineLayout.get(),							    // layout
		renderPass									// renderPass
	);

	pipeline = logicalDevice.createGraphicsPipelineUnique(nullptr, graphicsPipelineCreateInfo);
	
	return true;
}