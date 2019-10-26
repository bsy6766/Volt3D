/**
*	@file Pipeline.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Pipeline.h"

#include "Vulkan/Devices/LogicalDevice.h"
#include "Vulkan/SwapChain/Swapchain.h"
#include "ShaderModule.h"
#include "Shader/Shader.h"
#include "Renderer/Vertex.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Pipeline::Pipeline()
	: pipeline()
	, pipelineLayout()
	, viewport()
	, scissor()
	, vertexInputBindingDescription()
	, descriptorSetLayout( nullptr )
	, descriptorPool( nullptr )
	, pipelineMultisampleStateCreateInfo()
	, pipelineColorBlendAttachmentState()
	, pipelineColorBlendStateCreateInfo()
	, shaders()
	, shaderCreateInfos()
	// @todo init all
{}

Pipeline::~Pipeline()
{
	const vk::Device& logicalDevice = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice();
	logicalDevice.destroyDescriptorSetLayout( descriptorSetLayout );
	logicalDevice.destroyDescriptorPool( descriptorPool );
	logicalDevice.destroyPipelineLayout( pipelineLayout );
	logicalDevice.destroyPipeline( pipeline );

	shaders.clear();
}

bool Pipeline::init( const std::vector<std::filesystem::path>& shaderPath, const vk::Extent2D& extent, const vk::RenderPass& renderPass )
{
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

	pipeline = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice().createGraphicsPipeline( nullptr, graphicsPipelineCreateInfo );

	return true;
}

bool Pipeline::initDescriptorSetLayout()
{
	std::vector<vk::DescriptorSetLayoutBinding> bindings;
	std::vector<vk::DescriptorSetLayoutBinding> shaderBindings;

	for (auto& shader : shaders)
	{
		shaderBindings = shader->getDescriptorSetLayoutBinding();
		bindings.insert( bindings.end(), shaderBindings.begin(), shaderBindings.end() );
	}

	if (bindings.empty()) return false;

	vk::DescriptorSetLayoutCreateInfo layoutInfo
	(
		vk::DescriptorSetLayoutCreateFlags(),
		uint32_t(bindings.size()),
		bindings.data()
	);

	descriptorSetLayout = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice().createDescriptorSetLayout( layoutInfo );

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

	pipelineLayout = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice().createPipelineLayout( layoutCreateInfo );

	return true;
}

inline const vk::Pipeline& Pipeline::getVKPipeline() const 
{ 
	return pipeline; 
}

inline const vk::Viewport& Pipeline::getViewport() const 
{ 
	return viewport; 
}

inline const vk::Rect2D& Pipeline::getScissor() const 
{ 
	return scissor; 
}

inline const vk::PipelineLayout& Pipeline::getLayout() const 
{ 
	return pipelineLayout; 
}

inline const vk::DescriptorSetLayout& Pipeline::getDescriptorSetLayout() const 
{ 
	return descriptorSetLayout; 
}

std::shared_ptr<v3d::Shader> Pipeline::getShader( const vk::ShaderStageFlagBits stage ) const
{
	for (auto shader : shaders)
	{
		if (shader->getShaderModule().getStage() == stage)
		{
			return shader;
		}
	}

	return nullptr;
}

VK_NS_END
V3D_NS_END