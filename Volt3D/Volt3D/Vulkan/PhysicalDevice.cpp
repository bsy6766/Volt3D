/**
*	@file PhysicalDevice.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "PhysicalDevice.h"

#include "Surface.h"

#include <sstream>

v3d::vulkan::PhysicalDevice::PhysicalDevice(vk::PhysicalDevice&& physicalDevice, Surface& surface)
	: physicalDevice(physicalDevice)
	, graphicsQueueFamilyIndex(initGraphicsQueueFamilyIndex())
	, presentQueueFamilyIndex(initPresentsQueueFamilyIndex(surface))
{}

bool v3d::vulkan::PhysicalDevice::isSuitable(const vk::PhysicalDevice& physicalDevice)
{
	auto& logger = v3d::Logger::getInstance();

	static auto bitToStr = [](uint64_t size) -> std::string
	{
		const std::vector<std::string> SUFFIXES{ { "B", "KB", "MB", "GB", "TB", "PB" } };
		size_t suffixIndex = 0;
		while (suffixIndex < SUFFIXES.size() - 1 && size > 1024) {
			size >>= 10;
			++suffixIndex;
		}

		std::stringstream buffer;
		buffer << size << " " << SUFFIXES[suffixIndex];
		return buffer.str();
	};

	vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();

	if (properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
	{
		logger.info("Found discrete GPU");

		auto physicalDeviceMemoryProperties = physicalDevice.getMemoryProperties();

		logger.trace("GPU information ---");
		logger.trace("Driver Version: " + std::to_string(properties.driverVersion));
		logger.trace("Device Name: " + std::string(properties.deviceName));
		logger.trace("Device Type: VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU");
		const int major = (properties.apiVersion >> 22) & 0x3FF;
		const int minor = (properties.apiVersion >> 12) & 0x3FF;
		const int version = (properties.apiVersion & 0xFFF);
		logger.trace("API Version: " + std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(version));
		logger.trace("Memory heaps: " + std::to_string(physicalDeviceMemoryProperties.memoryHeapCount));
		for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryHeapCount; ++i)
		{
			const auto& heap = physicalDeviceMemoryProperties.memoryHeaps[i];
			logger.trace("\tHeap " + std::to_string(i) + " flags: " + vk::to_string(heap.flags) + ", size: " + std::to_string(heap.size) + " (" + bitToStr(heap.size) + ")");
		}
		logger.trace("Memory type: " + std::to_string(physicalDeviceMemoryProperties.memoryTypeCount));
		for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; ++i)
		{
			const auto& type = physicalDeviceMemoryProperties.memoryTypes[i];
			logger.trace("\tType " + std::to_string(i) + " flags: " + vk::to_string(type.propertyFlags) + ", heap: " + std::to_string(type.heapIndex));
		}
		logger.trace("-------------------");

		return true;
	}
	logger.critical("Failed to find discrete GPU");

	return true;
}

std::optional<uint32_t> v3d::vulkan::PhysicalDevice::initGraphicsQueueFamilyIndex()
{
	std::optional<uint32_t> gIndex;

	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
	if (queueFamilyProperties.empty()) return gIndex;

	uint32_t i = 0;
	for (auto& queueFamilyProperty : queueFamilyProperties)
	{
		if (queueFamilyProperty.queueCount > 0 && (queueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics))
		{
			gIndex.emplace(i);
			break;
		}

		i++;
	}

	return gIndex;
}

std::optional<uint32_t> v3d::vulkan::PhysicalDevice::initPresentsQueueFamilyIndex(v3d::vulkan::Surface& surface)
{
	std::optional<uint32_t> pIndex = 0;

	std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
	if (queueFamilyProperties.empty()) return pIndex;

	uint32_t i = 0;
	for (auto& queueFamilyProperty : queueFamilyProperties)
	{
		vk::Bool32 supportSurface = physicalDevice.getSurfaceSupportKHR(i, surface.getHandle().get());
		if (queueFamilyProperty.queueCount > 0 && supportSurface)
		{
			pIndex.emplace(i);
			break;
		}

		i++;
	}

	return pIndex;
}

vk::UniqueDevice v3d::vulkan::PhysicalDevice::createDeviceUnique(vk::DeviceCreateInfo& createInfo) const
{
	return std::move(physicalDevice.createDeviceUnique(createInfo));
}

std::vector<vk::QueueFamilyProperties> v3d::vulkan::PhysicalDevice::getQueueFamilyProperties() const
{
	return physicalDevice.getQueueFamilyProperties();
}

vk::SurfaceCapabilitiesKHR v3d::vulkan::PhysicalDevice::getSurfaceCapabilitiesKHR(const Surface& surface) const
{
	return physicalDevice.getSurfaceCapabilitiesKHR(reinterpret_cast<const vk::UniqueSurfaceKHR&>(surface).get());
}

vk::Bool32 v3d::vulkan::PhysicalDevice::getSurfaceSupportKHR(const uint32_t index, const Surface& surface) const
{
	return physicalDevice.getSurfaceSupportKHR(index, reinterpret_cast<const vk::UniqueSurfaceKHR&>(surface).get());
}

std::vector<vk::SurfaceFormatKHR> v3d::vulkan::PhysicalDevice::getSurfaceFormatsKHR(const Surface& surface) const
{
	return physicalDevice.getSurfaceFormatsKHR(reinterpret_cast<const vk::UniqueSurfaceKHR&>(surface).get());
}

std::vector<vk::PresentModeKHR> v3d::vulkan::PhysicalDevice::getSurfacePresentModesKHR(const Surface& surface) const
{
	return physicalDevice.getSurfacePresentModesKHR(reinterpret_cast<const vk::UniqueSurfaceKHR&>(surface).get());
}

vk::PhysicalDeviceProperties v3d::vulkan::PhysicalDevice::getProperties() const
{
	return physicalDevice.getProperties();
}

vk::PhysicalDeviceFeatures v3d::vulkan::PhysicalDevice::getFeatures() const
{
	return physicalDevice.getFeatures();
}

vk::PhysicalDeviceMemoryProperties v3d::vulkan::PhysicalDevice::getMemoryProperties() const
{
	return physicalDevice.getMemoryProperties();
}

std::vector<vk::ExtensionProperties> v3d::vulkan::PhysicalDevice::EnumerateDeviceExtensionProperties() const
{
	return physicalDevice.enumerateDeviceExtensionProperties();
}

std::vector<vk::LayerProperties> v3d::vulkan::PhysicalDevice::enumerateDeviceLayerProperties() const
{
	return physicalDevice.enumerateDeviceLayerProperties();
}

std::optional<uint32_t> v3d::vulkan::PhysicalDevice::getGraphicsQueueFamilyIndex() const
{
	return graphicsQueueFamilyIndex;
}

std::optional<uint32_t> v3d::vulkan::PhysicalDevice::getPresentQueueFamilyIndex() const
{
	return presentQueueFamilyIndex;
}
