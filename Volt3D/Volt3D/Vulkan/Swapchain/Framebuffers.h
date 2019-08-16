/**
*	@file Framebuffers.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_FRAMEBUFFERS_H
#define V3D_VK_FRAMEBUFFERS_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

#include <vector>

V3D_NS_BEGIN
VK_NS_BEGIN

class Swapchain;

/**
*	@class Framebuffer
*	@brief A wrapper for Vulkan framebuffer
*
*	@group Vulkan
*	
*	@since 1.0
*/
class VOLT3D_DLL Framebuffers
{
	friend class Context;

private:
	Framebuffers() = delete;
	Framebuffers( const v3d::vulkan::Swapchain& swapchain, const vk::RenderPass& renderPass );
	
	std::vector<vk::Framebuffer> framebuffers;

public:
	~Framebuffers();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Framebuffers );
	DEFAULT_MOVE_CONSTRUCTORS( Framebuffers );

	// [] operator
	const vk::Framebuffer& operator[] ( const std::size_t index );

	/** Get size of framebuffers */
	const std::size_t size() const;

	/** Get vulkan framebuffers */
	const std::vector<vk::Framebuffer>& getFramebuffers() const;
};

VK_NS_END
V3D_NS_END

#endif