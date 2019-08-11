/**
*	@file Buffer.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_BUFFER_H
#define V3D_VK_BUFFER_H

#include <vulkan/vulkan.hpp>

#include "Utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

/**
*	@class Buffer
*	@brief A wrapper for Vulkan's Buffer and DeviceMemory. Base class for other buffer type classes
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL Buffer
{
protected:
	const vk::Device& logicalDevice;
	vk::Buffer buffer;
	vk::DeviceMemory deviceMemory;
	vk::DeviceSize size;

	void initBuffer( const vk::BufferUsageFlags usageFlags );
	void initDeviceMemory( const vk::MemoryPropertyFlags memoryPropertyFlags );
	void copyData( const void* data );

public:
	/** Deleted default constructor */
	Buffer() = delete;

	/**
	*	Buffer
	*	Creates Vulkan Buffer and DeviceMemory
	*	@param size The size of buffer data
	*	@param usageFlag A Vulakn buffer usage flag
	*	@param memoryPropertyFlags A Vulkan memory property flag
	*	@param [optional] data A pointer to buffer data
	*	@note Data is copied if data pointer is not nullptr.
	*/
	Buffer( const vk::DeviceSize size, const vk::BufferUsageFlags usageFlags, const vk::MemoryPropertyFlags memoryPropertiyFlags, const void* data = nullptr );

	/** Destructor */
	virtual ~Buffer();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Buffer );
	DEFAULT_MOVE_CONSTRUCTORS( Buffer );

	/** 
	*	Map buffer memory
	*	@return A pointer to data pointer where buffer is mapped
	*/
	void * mapMemory() const;
	
	/** Unmap buffer memory */
	void unmapMemory() const;

	/** Get Vulkan Buffer */
	const vk::Buffer& getBuffer() const;

	/** Get Vulkan DeviceMemory */
	const vk::DeviceMemory& getDeviceMemory() const;

	/** Get size of buffer */
	const vk::DeviceSize& getSize() const;
};

VK_NS_END
V3D_NS_END

#endif