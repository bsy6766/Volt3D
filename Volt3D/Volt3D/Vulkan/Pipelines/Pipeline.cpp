/**
*	@file Pipeline.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Pipeline.h"

#include "Vulkan/Context.h"
#include "Vulkan/Devices/LogicalDevice.h"
#include "Vulkan/SwapChain/Swapchain.h"
#include "Renderer/Vertex.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Pipeline::Pipeline()
	: logicalDevice( v3d::vulkan::Context::get()->getLogicalDevice()->get() )
	, pipeline( nullptr )
	, viewport()
	, scissor()
	, descriptorSetLayout( nullptr )
	, descriptorPool( nullptr )
	// @todo init all
{}

Pipeline::~Pipeline()
{
	for (auto& shader : shaders) { shader.second.release(); }
	shaders.clear();
	logicalDevice.destroyDescriptorSetLayout( descriptorSetLayout );
	logicalDevice.destroyPipelineLayout( pipelineLayout );
	logicalDevice.destroyPipeline( pipeline );
}

const vk::Pipeline& Pipeline::get() const { return pipeline; }

const vk::Viewport& Pipeline::getViewport() const { return viewport; }

const vk::Rect2D& Pipeline::getScissor() const { return scissor; }

const vk::PipelineLayout& Pipeline::getLayout() const { return pipelineLayout; }

bool Pipeline::init( const std::vector<std::filesystem::path>& shaderPath, const vk::Extent2D& extent, const vk::RenderPass& renderPass )
{
	if (!initShaderProgram( shaderPath )) return false;
	if (!initDescriptorSetLayout()) return false;
	if (!initPipelineLayout()) return false;

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
	
	viewport = vk::Viewport
	(
		0.0f, 0.0f,
		static_cast<float>(extent.width),
		static_cast<float>(extent.height),
		0.0f, 1.0f
	);

	scissor = vk::Rect2D( vk::Offset2D( 0, 0 ), extent );

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

	// typical blending
	pipelineColorBlendAttachmentState = vk::PipelineColorBlendAttachmentState
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

	pipelineColorBlendStateCreateInfo = vk::PipelineColorBlendStateCreateInfo
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
		uint32_t( shaderCreateInfos.size() ),		// stageCount
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
		pipelineLayout,							    // layout
		renderPass									// renderPass
	);

	pipeline = logicalDevice.createGraphicsPipeline( nullptr, graphicsPipelineCreateInfo );

	return true;
}

bool Pipeline::initShaderProgram( const std::vector<std::filesystem::path>& shaderPath )
{
	// Requires at least 2 shaders (vert & frag)
	if (shaderPath.size() < 2) return false;

	// Create shader instance
	for (auto& path : shaderPath)
	{
		const auto pathStr = path.string();
		shaders.emplace( v3d::vulkan::Shader::toShaderStageFlagbits( pathStr ), std::move( v3d::vulkan::Shader( pathStr ) ) );
	}

	// Init shaders
	for (auto& e : shaders)
	{
		if (!(e.second).init()) return false;
		shaderCreateInfos.push_back( (e.second).getPipelineShaderStageCreateInfo() );
	}

	return true;
}

bool Pipeline::initDescriptorSetLayout()
{
	std::vector<vk::DescriptorSetLayoutBinding> bindings;
	std::vector<vk::DescriptorSetLayoutBinding> shaderBindings;

	for (auto& [stage, shader] : shaders)
	{
		shaderBindings = shader.getDescriptorSetLayoutBinding();
		bindings.insert( bindings.end(), shaderBindings.begin(), shaderBindings.end() );
	}

	if (bindings.empty()) return false;

	vk::DescriptorSetLayoutCreateInfo layoutInfo
	(
		vk::DescriptorSetLayoutCreateFlags(),
		uint32_t(bindings.size()),
		bindings.data()
	);

	descriptorSetLayout = logicalDevice.createDescriptorSetLayout( layoutInfo );

	return true;
}

bool Pipeline::initPipelineLayout()
{
	vk::PipelineLayoutCreateInfo layoutCreateInfo
	(
		vk::PipelineLayoutCreateFlags(),
		1, 
		&descriptorSetLayout,
		0, 
		nullptr
	);

	pipelineLayout = logicalDevice.createPipelineLayout( layoutCreateInfo );

	return true;
}

VK_NS_END
V3D_NS_END