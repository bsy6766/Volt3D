/**
*	@file Swapchain.h
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

class DepthImage;

/**
*	@class Swapchain
*	@brief Wrapper for Vulkan's swapchain.
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL Swapchain
{
	friend class Context;

private:
	Swapchain();

	vk::SwapchainKHR swapchain;
	vk::SurfaceFormatKHR surfaceFormat;
	vk::Extent2D extent;

	std::vector<vk::Image> images;
	std::vector<vk::ImageView> imageViews;

	v3d::vulkan::DepthImage* depthImage;

	vk::SurfaceFormatKHR selectSurfaceFormat( const std::vector<vk::SurfaceFormatKHR>& surfaceFormats ) const;
	vk::Extent2D selectExtent( const vk::SurfaceCapabilitiesKHR& surfaceCapabilities, const glm::ivec2& frameBufferSize ) const;
	vk::PresentModeKHR selectPresentMode( const std::vector<vk::PresentModeKHR>& presentModes ) const;

	bool init();

public:
	~Swapchain();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Swapchain );
	DEFAULT_MOVE_CONSTRUCTORS( Swapchain );
	
	/** Get Vulkan SwapchainKHR */
	const vk::SwapchainKHR& getSwapchainKHR() const;

	/** Get format of swapchain */
	const vk::Format& getFormat() const;

	/** Get extent of swapchain */
	const vk::Extent2D& getExtent() const;

	/** Get images */
	const std::vector<vk::Image>& getImages() const;

	/** Get image views */
	const std::vector<vk::ImageView>& getImageViews() const;

	/** Get depth image */
	const v3d::vulkan::DepthImage* getDepthImage() const;
};

VK_NS_END
V3D_NS_END

#endif