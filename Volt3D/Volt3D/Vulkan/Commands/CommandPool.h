/**
*	@file CommandPool.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_COMMAND_POOL_H
#define V3D_VK_COMMAND_POOL_H

#include <vulkan/vulkan.hpp>

#include "Utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

/**
*	@class CommandPool
*	@brief A wrapper for Vulkan's CommandPool.
*
*	@group Vulkan
*
*	@since 1.0
*/
class CommandPool
{
	friend class Context;

private:
	CommandPool() = delete;
	CommandPool( const uint32_t queueIndex );

	vk::CommandPool commandPool;

public:
	~CommandPool();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( CommandPool );
	DEFAULT_MOVE_CONSTRUCTORS( CommandPool );

	/** Get Vulkan CommandPool */
	const vk::CommandPool& getVKCommandPool() const;
};

VK_NS_END
V3D_NS_END

#endif