/**
*	@file ShaderState.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_SHADER_STATE_H
#define V3D_VK_SHADER_STATE_H

#include <vulkan/vulkan.hpp>

#include <unordered_map>
#include <optional>

#include "Uniform.h"
#include "UniformBlock.h"
#include "Attribute.h"
#include "utils/Macros.h"

namespace glslang { class TProgram; }

V3D_NS_BEGIN
VK_NS_BEGIN

/**
*	@class ShaderState
*	@brief A class that contains all uniform information of shader
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL ShaderState
{
	friend class Shader;

private:
	ShaderState();

	// Attributes in shaders
	std::unordered_map<uint32_t/*location*/, v3d::vulkan::Attribute> attributes;

	// Uniform blocks in shaders
	std::unordered_map<uint32_t/*binding*/, v3d::vulkan::UniformBlock> uniformBlocks;

	// Uniforms in shader
	std::unordered_map<uint32_t/*binding*/, v3d::vulkan::Uniform> uniforms;

	// initialize
	void init( glslang::TProgram& program );

public:
	~ShaderState();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( ShaderState );
	DEFAULT_MOVE_CONSTRUCTORS( ShaderState );

	/**
	*	Get uniform block by binding
	*	@param binding A uniform block binding defined in shader.
	*	@return A pointer to uniform block with matching binding. Else, std::nullopt.
	*/
	std::optional<std::reference_wrapper<v3d::vulkan::UniformBlock>> getUniformBlock( const uint32_t binding );

	/**
	*	Get uniform block by name
	*	@param binding A uniform block name defined in shader.
	*	@return A uniform block with matching name. Else, std::nullopt.
	*/
	std::optional<std::reference_wrapper<v3d::vulkan::UniformBlock>> getUniformBlock( const std::string_view name );
};

VK_NS_END
V3D_NS_END

#endif