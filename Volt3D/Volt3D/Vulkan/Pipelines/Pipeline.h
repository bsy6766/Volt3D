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

#include "Shader.h"
#include "utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class SwapChain;

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
	const vk::Device& logicalDevice;
	vk::PipelineLayout pipelineLayout;
	vk::Pipeline pipeline;
	vk::Viewport viewport;
	vk::Rect2D scissor;

	std::vector<vk::VertexInputAttributeDescription> vertexInputAttribDescriptions;
	vk::VertexInputBindingDescription vertexInputBindingDescription;

	std::unordered_map<vk::ShaderStageFlagBits, v3d::vulkan::Shader> shaders;
	std::vector<vk::PipelineShaderStageCreateInfo> shaderCreateInfos;

	bool init(const vk::Device& logicalDevice, const v3d::vulkan::SwapChain& swapChain, const vk::RenderPass& renderPass, const vk::DescriptorSetLayout& descriptorSetLayout);

public:
	Pipeline();
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Pipeline);
	DEFAULT_MOVE_CONSTRUCTORS(Pipeline);
	~Pipeline();
	
	const vk::Pipeline& get() const;
	const vk::Viewport& getViewport() const;
	const vk::Rect2D& getScissor() const;
	const vk::PipelineLayout& getLayout() const;
};

VK_NS_END
V3D_NS_END

#endif