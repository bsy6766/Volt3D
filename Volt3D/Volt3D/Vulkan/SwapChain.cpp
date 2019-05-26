#include <PreCompiled.h>

#include <algorithm>

#include "SwapChain.h"

#include "PhysicalDevice.h"
#include "Device.h"

SwapChain::SwapChain()
	: swapChain( nullptr )
{}

bool SwapChain::init( PhysicalDevice& physicalDevice, Device& device, Surface& surface )
{
	std::vector<vk::SurfaceFormatKHR> formats = physicalDevice.getSurfaceFormatsKHR( surface );
	if( formats.empty() ) return false;

	vk::Format format = (formats.front().format == vk::Format::eUndefined) ? vk::Format::eB8G8R8A8Unorm : formats.front().format;

	vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR( surface );
	vk::Extent2D swapChainExtent;
	if( surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max() )
	{
		// If the surface size is undefined, the size is set to the size of the images requested.
		swapChainExtent.width = std::clamp( 1280u, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width );
		swapChainExtent.height = std::clamp( 720u, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height );
	}
	else
	{
		// If the surface size is defined, the swap chain size must match
		swapChainExtent = surfaceCapabilities.currentExtent;
	}

	// The FIFO present mode is guaranteed by the spec to be supported
	vk::PresentModeKHR swapchainPresentMode = vk::PresentModeKHR::eFifo;
	// Visit again to learn more about present mode.

	vk::SurfaceTransformFlagBitsKHR preTransform = (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) ? vk::SurfaceTransformFlagBitsKHR::eIdentity : surfaceCapabilities.currentTransform;

	vk::CompositeAlphaFlagBitsKHR compositeAlpha =
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied :
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied :
		(surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) ? vk::CompositeAlphaFlagBitsKHR::eInherit : vk::CompositeAlphaFlagBitsKHR::eOpaque;

	vk::SwapchainCreateInfoKHR swapChainCreateInfo
	(
		vk::SwapchainCreateFlagsKHR(),
		reinterpret_cast<const vk::UniqueSurfaceKHR&>(surface).get(),
		surfaceCapabilities.minImageCount, 
		format, 
		vk::ColorSpaceKHR::eSrgbNonlinear,
		swapChainExtent,
		1,
		vk::ImageUsageFlagBits::eColorAttachment,
		vk::SharingMode::eExclusive,
		0,
		nullptr,
		preTransform,
		compositeAlpha, 
		swapchainPresentMode,
		true, 
		nullptr
	);

	uint32_t queueFamilyIndices[2] = { device.getGraphicsQueueFamilyIndex(), device.getPresentQueueFamilyIndex() };
	if( queueFamilyIndices[0] != queueFamilyIndices[1] )
	{
		// If the graphics and present queues are from different queue families, we either have to explicitly transfer ownership of images between
		// the queues, or we have to create the swapchain with imageSharingMode as VK_SHARING_MODE_CONCURRENT
		swapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
		swapChainCreateInfo.queueFamilyIndexCount = 2;
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}

	//swapChain = device.createSwapchainKHRUnique( swapChainCreateInfo );

	//std::vector<vk::Image> swapChainImages = device.getSwapchainImagesKHR( swapChain.get() );

	//imageViews.reserve( swapChainImages.size() );
	//vk::ComponentMapping componentMapping( vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA );
	//vk::ImageSubresourceRange subResourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 );
	//for( auto image : swapChainImages )
	//{
	//	vk::ImageViewCreateInfo imageViewCreateInfo( vk::ImageViewCreateFlags(), image, vk::ImageViewType::e2D, format, componentMapping, subResourceRange );
	//	imageViews.push_back( std::move( device.createImageViewUnique( imageViewCreateInfo ) ) );
	//}

	return true;
}
