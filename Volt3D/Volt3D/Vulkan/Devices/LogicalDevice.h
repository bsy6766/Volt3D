/**
*	@file LogicalDevice.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_LOGICAL_DEVICE_H
#define V3D_VK_LOGICAL_DEVICE_H

#include <vulkan/vulkan.hpp>

#include <vector>

#include "Utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class Context;

/**
*	@class LogicalDevice
*	@brief A wrapper for Vulakn LogicalDevice
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL LogicalDevice
{
	friend class Context;

private:
	LogicalDevice();

	vk::Device logicalDevice;

	uint32_t graphicsQueueFamilyIndex;
	uint32_t computeQueueFamilyIndex;
	uint32_t transferQueueFamilyIndex;
	uint32_t presentQueueFamilyIndex;

	vk::Queue graphicsQueue;
	vk::Queue computeQueue;
	vk::Queue transferQueue;
	vk::Queue presentQueue;

	bool init( const vk::SurfaceKHR& surface, const vk::PhysicalDevice& physicalDevice );

public:
	~LogicalDevice();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( LogicalDevice );
	DEFAULT_MOVE_CONSTRUCTORS( LogicalDevice );

	/** Get LogicalDevice */
	static v3d::vulkan::LogicalDevice* get();

	/** Get Vulkan LogicalDevice */
	const vk::Device& getVKLogicalDevice() const;

	/** Get graphics queue family index */
	uint32_t getGraphicsQueueFamilyIndex() const;

	/** Get compute queue family index */
	uint32_t getComputeQueueFamilyIndex() const;

	/** Get transfer queue family index */
	uint32_t getTransferQueueFamilyIndex() const;

	/** Get present queue family index */
	uint32_t getPresentQueueFamilyIndex() const;

	/** Get graphics queue */
	const vk::Queue& getGraphicsQueue() const;

	/** Get compute queue */
	const vk::Queue& getComputeQueue() const;

	/** Get transfer queue */
	const vk::Queue& getTransferQueue() const;

	/** Get present queue */
	const vk::Queue& getPresentQueue() const;

	/** Log LogicalDevice */
	void print() const;
};

VK_NS_END
V3D_NS_END

#endif