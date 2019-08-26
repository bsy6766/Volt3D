/**
*	@file Pipeline.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_PIPELINE_H
#define V3D_VK_PIPELINE_H

#include <vulkan/vulkan.hpp>

#include <unordered_map>

#include "utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class Shader;

/**
*	@class Pipeline
*	@brief Wrapper of Vulkan's Pipeline
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL Pipeline
{
	friend class Context;

private:
	vk::DescriptorSetLayout descriptorSetLayout;
	vk::DescriptorPool descriptorPool;

	vk::PipelineLayout pipelineLayout;
	vk::Pipeline pipeline;
	vk::Viewport viewport;
	vk::Rect2D scissor;

	std::vector<vk::VertexInputAttributeDescription> vertexInputAttribDescriptions;
	vk::VertexInputBindingDescription vertexInputBindingDescription;

	// @todo Does pipeline actually need to have these stuffs?
	vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;
	vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;
	vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo;

	std::unordered_map<vk::ShaderStageFlagBits, v3d::vulkan::Shader*> shaders;
	std::vector<vk::PipelineShaderStageCreateInfo> shaderCreateInfos;

	bool init( const std::vector<std::filesystem::path>& shaderPath, const vk::Extent2D& extent, const vk::RenderPass& renderPass );
	bool initShaderProgram( const std::vector<std::filesystem::path>& shaderPath );
	bool initDescriptorSetLayout();
	bool initPipelineLayout();

public:
	Pipeline();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Pipeline );
	DEFAULT_MOVE_CONSTRUCTORS( Pipeline );

	~Pipeline();

	/** Get Vulkan Pipeline */
	const vk::Pipeline& getVKPipeline() const;

	/** Get view port of pipeline */
	const vk::Viewport& getViewport() const;

	/** Get scissor of pipeline */
	const vk::Rect2D& getScissor() const;

	/** Get Vulkan PipelineLayout */
	const vk::PipelineLayout& getLayout() const;

	/** Get DescriptorSetLayout */
	const vk::DescriptorSetLayout& getDescriptorSetLayout() const;

	/** Get shader from specific shader stage */
	v3d::vulkan::Shader* getShader( const vk::ShaderStageFlagBits stage ) const;
};

VK_NS_END
V3D_NS_END

#endif