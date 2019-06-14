/**
*	@file Pipeline.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Pipeline.h"

#include "Device.h"
#include "ShaderModule.h"
#include "SwapChain.h"
#include "Vertex.h"

v3d::vulkan::Pipeline::Pipeline()
	: pipelineLayout()
	, pipeline()
	, viewport()
	, scissor()
{}

vk::Viewport v3d::vulkan::Pipeline::getViewport() const
{
	return viewport;
}

vk::Rect2D v3d::vulkan::Pipeline::getScissor() const
{
	return scissor;
}

bool v3d::vulkan::Pipeline::init(const v3d::vulkan::Device& device, const v3d::vulkan::SwapChain& swapChain, const vk::RenderPass& renderPass)
{
	vk::PipelineLayoutCreateInfo layoutCreateInfo
	(
		vk::PipelineLayoutCreateFlags(),
		0, nullptr,
		0, nullptr
	);

	pipelineLayout = device.createPipelineLayoutUnique(layoutCreateInfo);

	v3d::vulkan::ShaderModule vertShader;
	if (!vertShader.init("Shaders/vert.spv", device)) return false;

	v3d::vulkan::ShaderModule fragShader;
	if (!fragShader.init("Shaders/frag.spv", device)) return false;

	vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfos[2] =
	{
	  vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eVertex, vertShader.get(), "main"),
	  vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(), vk::ShaderStageFlagBits::eFragment, fragShader.get(), "main")
	};
	
	std::vector<vk::VertexInputAttributeDescription> vertexInputAttribDescriptions = v3d::vulkan::Vertex::getInputAttributeDescription();
	vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo
	(
		vk::PipelineVertexInputStateCreateFlags(),
		1, &v3d::vulkan::Vertex::getInputBindingDescription(),
		2, vertexInputAttribDescriptions.data()
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
	vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState
	(
		false,                      // blendEnable
		vk::BlendFactor::eZero,     // srcColorBlendFactor
		vk::BlendFactor::eZero,     // dstColorBlendFactor
		vk::BlendOp::eAdd,          // colorBlendOp
		vk::BlendFactor::eZero,     // srcAlphaBlendFactor
		vk::BlendFactor::eZero,     // dstAlphaBlendFactor
		vk::BlendOp::eAdd,          // alphaBlendOp
		colorComponentFlags         // colorWriteMask
	);

	// typical blending
	//vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState
	//(
	//	true,									// blendEnable
	//	vk::BlendFactor::eSrcAlpha,				// srcColorBlendFactor
	//	vk::BlendFactor::eOneMinusSrcAlpha,     // dstColorBlendFactor
	//	vk::BlendOp::eAdd,						// colorBlendOp
	//	vk::BlendFactor::eOne,					// srcAlphaBlendFactor
	//	vk::BlendFactor::eZero,					// dstAlphaBlendFactor
	//	vk::BlendOp::eAdd,						// alphaBlendOp
	//	colorComponentFlags						// colorWriteMask
	//);

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
		2,                                          // stageCount
		pipelineShaderStageCreateInfos,             // pStages
		&pipelineVertexInputStateCreateInfo,        // pVertexInputState
		&pipelineInputAssemblyStateCreateInfo,      // pInputAssemblyState
		nullptr,                                    // pTessellationState
		&pipelineViewportStateCreateInfo,           // pViewportState
		&pipelineRasterizationStateCreateInfo,      // pRasterizationState
		&pipelineMultisampleStateCreateInfo,        // pMultisampleState
		nullptr,							        // pDepthStencilState
		&pipelineColorBlendStateCreateInfo,         // pColorBlendState
		&pipelineDynamicStateCreateInfo,            // pDynamicState
		pipelineLayout.get(),                       // layout
		renderPass									// renderPass
	);

	pipeline = device.createPipelineUnique(graphicsPipelineCreateInfo);

	return true;
}
