/**
*	@file Surface.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include <algorithm>

#include "SwapChain.h"

#include "PhysicalDevice.h"
#include "Device.h"

v3d::vulkan::SwapChain::SwapChain(vk::UniqueSwapchainKHR&& swapChain)
	: swapChain(std::move(swapChain))
{
	std::vector<vk::Image> swapChainImages = device.getSwapchainImagesKHR(swapChain.get());

	imageViews.reserve(swapChainImages.size());
	vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA);
	vk::ImageSubresourceRange subResourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
	for (auto image : swapChainImages)
	{
		vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, format, componentMapping, subResourceRange);
		imageViews.push_back(std::move(device.createImageViewUnique(imageViewCreateInfo)));
	}
}
