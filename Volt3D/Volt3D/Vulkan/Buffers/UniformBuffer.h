/**
*	@file UniformBuffer.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_UNIFORM_BUFFER_H
#define V3D_VK_UNIFORM_BUFFER_H

#include <vulkan/vulkan.hpp>

#include "Buffer.h"
#include "Utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

/**
*	@class UniformBuffer
*	@brief A buffer that holds uniforms. Derives v3d::vulkan::Buffer.
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL UniformBuffer : public v3d::vulkan::Buffer
{
public:
	UniformBuffer() = delete;
	UniformBuffer( const uint32_t size, const vk::BufferUsageFlags usageFlags, const vk::MemoryPropertyFlags memoryPropertiyFlags, const void* data = nullptr );
	~UniformBuffer();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( UniformBuffer );
	DEFAULT_MOVE_CONSTRUCTORS( UniformBuffer );

	/**
	*	Update uniform data.
	*	@param data [in] A data pointer of uniform buffer
	*/
	void update( const void* data );

	vk::DescriptorBufferInfo getDescriptorBufferInfo() const;
};

VK_NS_END
V3D_NS_END

#endif