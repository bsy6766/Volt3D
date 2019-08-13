/**
*	@file SwapChain.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_SWAPCHAIN_H
#define V3D_VK_SWAPCHAIN_H

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

/**
*	@class SwapChain
*	@brief Wrapper for Vulkan's swapchain.
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL SwapChain
{
	friend class Context;

private:
	SwapChain();

	const vk::Device& logicalDevice;

	vk::SwapchainKHR swapchain;
	vk::SurfaceFormatKHR surfaceFormat;
	vk::Extent2D extent;

	vk::SurfaceFormatKHR selectSurfaceFormat( const std::vector<vk::SurfaceFormatKHR>& surfaceFormats ) const;
	vk::Extent2D selectExtent( const vk::SurfaceCapabilitiesKHR& surfaceCapabilities, const glm::ivec2& frameBufferSize ) const;
	vk::PresentModeKHR selectPresentMode( const std::vector<vk::PresentModeKHR>& presentModes ) const;

	bool init();
	//bool initImageViews();

public:
	~SwapChain();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( SwapChain );
	DEFAULT_MOVE_CONSTRUCTORS( SwapChain );
	
	/** Get Vulkan SwapchainKHR */
	const vk::SwapchainKHR& get() const;

	/** Get format of swapchain */
	const vk::Format& getFormat() const;

	/** Get extent of swapchain */
	const vk::Extent2D& getExtent2D() const;
};

VK_NS_END
V3D_NS_END

#endif