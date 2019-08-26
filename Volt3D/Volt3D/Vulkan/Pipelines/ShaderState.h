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

#include "utils/Macros.h"

namespace glslang { class TProgram; }

V3D_NS_BEGIN
VK_NS_BEGIN

class Attribute;
class UniformBlock;
class Uniform;

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
	std::unordered_map<uint32_t/*location*/, v3d::vulkan::Attribute*> attributes;

	// Uniform blocks in shaders
	std::unordered_map<uint32_t/*binding*/, v3d::vulkan::UniformBlock*> uniformBlocks;

	// Uniforms in shader
	std::unordered_map<uint32_t/*binding*/, v3d::vulkan::Uniform*> uniforms;

	// initialize
	void init( glslang::TProgram& program );

public:
	~ShaderState();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( ShaderState );
	DEFAULT_MOVE_CONSTRUCTORS( ShaderState );

	/**
	*	Get attribute by location.
	*	@param location An attribute location in shader.
	*	@return A attribute with matching location. Else, nullptr.
	*/
	v3d::vulkan::Attribute* getAttribute( const uint32_t location ) const;

	/**
	*	Get attribute by name.
	*	@param name A attribute name defined in shader.
	*	@return A attribute with matching name. Else, nullptr.
	*/
	v3d::vulkan::Attribute* getAttribute( const std::string_view name ) const;

	/**
	*	Get uniform block by binding
	*	@param binding A uniform block binding defined in shader.
	*	@return A uniform block with matching binding. Else, nullptr.
	*/
	v3d::vulkan::UniformBlock* getUniformBlock( const uint32_t binding ) const;

	/**
	*	Get uniform block by name
	*	@param binding A uniform block name defined in shader.
	*	@return A uniform block with matching name. Else, nullptr.
	*/
	v3d::vulkan::UniformBlock* getUniformBlock( const std::string_view name ) const;

	/**
	*	Get uniform by binding.
	*	@warning This does not return uniform inside the uniform block.
	*	@see getUniformBlock
	*	@param binding A uniform binding defined in shader.
	*	@return A uniform with matching binding. Else, nullptr.
	*/
	v3d::vulkan::Uniform* getUniform( const uint32_t binding ) const;

	/**
	*	Get uniform by name.
	*	@warning This does not return uniform inside the uniform block.
	*	@see getUniformBlock
	*	@param name A uniform name defined in shader.
	*	@return A uniform with matching name. Else, nullptr.
	*/
	v3d::vulkan::Uniform* getUniform( const std::string_view name ) const;
};

VK_NS_END
V3D_NS_END

#endif