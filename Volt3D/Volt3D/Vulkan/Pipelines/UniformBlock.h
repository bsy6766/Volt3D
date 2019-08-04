/**
*	@file UniformBLock.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_UNIFORM_BLOCK_H
#define V3D_VK_UNIFORM_BLOCK_H

#include <Vulkan/vulkan.hpp>

#include "Uniform.h"
#include "utils/Macros.h"

#include <string>
#include <unordered_map>

V3D_NS_BEGIN
VK_NS_BEGIN

class Shader;

/**
*	@class UniformBlock
*	@brief A dynamically generated uniform block contains uniforms in specific shader
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL UniformBlock
{
	friend class v3d::vulkan::Shader;

private:
	UniformBlock() = delete;
	UniformBlock( const std::string& name, const int32_t binding, const int32_t size );

	// Name of uniform block
	std::string name;

	// Binding defined by shader
	int32_t binding;

	// Total size of uniform block
	int32_t size;

	// For uniform block, offset is -1
	//int32_t offset;
	// For uniform block, type is 0xffffffff
	//int32_t glType;

	// All uniforms in this uniform block
	std::unordered_map<std::string, v3d::vulkan::Uniform> uniforms;
	
public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( UniformBlock );
	DEFAULT_MOVE_CONSTRUCTORS( UniformBlock );

	~UniformBlock();

	/**
	*	Check if this uniform block has uniform by name.
	*	@param name Uniform name in shader.
	*/
	bool hasUniform( const std::string& name ) const;

	/** Get uniform block name */
	std::string getName() const;

	/** Get uniform block binding */
	int32_t getBinding() const;

	/** Get size of uniform block */
	int32_t getSize() const;

	/** Get total uniforms in uniform block */
	std::size_t getCount() const;
	
	/** 
	*	Log UniformBlock 
	*	@param detail Logs all uniform details if true. Else, does nothing.
	*/
	void print( const bool detail = false );
};

VK_NS_END
V3D_NS_END


#endif