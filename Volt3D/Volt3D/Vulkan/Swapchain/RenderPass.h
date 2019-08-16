/**
*	@file RenderPass.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_RENDER_PASS_H
#define V3D_VK_RENDER_PASS_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

#include <vector>

V3D_NS_BEGIN
VK_NS_BEGIN

/**
*	@class RenderPass
*	@brief A wrapper for Vulkan RenderPass
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL RenderPass
{
	friend class Context;

private:
	RenderPass() = delete;
	RenderPass( const vk::Format& format );

	vk::RenderPass renderPass;

public:
	~RenderPass();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( RenderPass );
	DEFAULT_MOVE_CONSTRUCTORS( RenderPass );

	/** Get Vulakn RenderPass */
	const vk::RenderPass& get() const;
};

VK_NS_END
V3D_NS_END

#endif