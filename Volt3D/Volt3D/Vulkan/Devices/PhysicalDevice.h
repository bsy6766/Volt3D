/**
*	@file PhysicalDevice.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_PHYSICAL_DEVICE_H
#define V3D_VK_PHYSICAL_DEVICE_H

#include <vulkan/vulkan.hpp>

#include <vector>

#include "Utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class Context;

/**
*	@class PhysicalDevice
*	@brief A wrapper for Vulakn PhysicalDevice
*
*	@group Vulkan
*	
*	@since 1.0
*/
class VOLT3D_DLL PhysicalDevice
{
	friend class Context;

private:
	PhysicalDevice();

	vk::PhysicalDevice physicalDevice;

	vk::PhysicalDeviceProperties properties;
	vk::PhysicalDeviceFeatures features;
	vk::PhysicalDeviceFeatures enabledFeatures;
	vk::PhysicalDeviceMemoryProperties memoryProperties;
	vk::SampleCountFlagBits sampleCountFlagBits;

	bool init( const std::vector<vk::PhysicalDevice>& physicalDevices );
	bool check( const vk::PhysicalDevice& physicalDevice );
	void initSampleCount();
	void initEnabledFeatures();

public:
	~PhysicalDevice();

	/** Get PhysicalDevice */
	const vk::PhysicalDevice& get() const;

	/** Get PhysicalDevice properties */
	const vk::PhysicalDeviceProperties& getProperties() const;

	/** Get PhysicalDevice Features */
	const vk::PhysicalDeviceFeatures& getFeatures() const;

	/** Get enabled features */
	const vk::PhysicalDeviceFeatures& getEnabledFeatures() const;

	/** Get PhysicalDevice MemoryProperties */
	const vk::PhysicalDeviceMemoryProperties& getMemoryProperties() const;

	/** Get PhysicalDevice SampleCountFlagBits */
	const vk::SampleCountFlagBits& getSampleCountFlagBits() const;

	/** Get memory type index */
	uint32_t getMemoryTypeIndex( const uint32_t memoryTypeBits, const vk::MemoryPropertyFlags memoryPropertyFlags ) const;

	/** Log PhysicalDevice */
	void print( const bool detail ) const;
};

VK_NS_END
V3D_NS_END

#endif