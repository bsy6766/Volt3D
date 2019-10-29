/**
*	@file Pipeline.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_PIPELINE_H
#define V3D_VK_PIPELINE_H

#include <vulkan/vulkan.hpp>

#include <vector>
#include <memory>

#include "utils/Macros.h"

V3D_NS_BEGIN
class Shader;
VK_NS_BEGIN

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

	std::vector<std::shared_ptr<v3d::Shader>> shaders;
	std::vector<vk::PipelineShaderStageCreateInfo> shaderCreateInfos;

	bool init( const std::vector<std::shared_ptr<v3d::Shader>>& shaders, const vk::Extent2D& extent, const vk::RenderPass& renderPass );
	bool initDescriptorSetLayout();
	bool initPipelineLayout();

public:
	Pipeline();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Pipeline );
	DEFAULT_MOVE_CONSTRUCTORS( Pipeline );

	~Pipeline();

	static Pipeline* create( const std::vector<std::shared_ptr<v3d::Shader>>& shaders, const vk::Extent2D& extent, const vk::RenderPass& renderPass );

	/** Get Vulkan Pipeline */
	inline const vk::Pipeline& getVKPipeline() const;

	/** Get view port of pipeline */
	inline const vk::Viewport& getViewport() const;

	/** Get scissor of pipeline */
	inline const vk::Rect2D& getScissor() const;

	/** Get Vulkan PipelineLayout */
	inline const vk::PipelineLayout& getLayout() const;

	/** Get DescriptorSetLayout */
	inline const vk::DescriptorSetLayout& getDescriptorSetLayout() const;

	/** Get shader from specific shader stage */
	std::shared_ptr<v3d::Shader> getShader( const vk::ShaderStageFlagBits stage ) const;
};

VK_NS_END
V3D_NS_END

#endif