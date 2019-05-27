/**
*	@file Device.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Device.h"

v3d::vulkan::Device::Device(vk::UniqueDevice&& device)
	: device(std::move(device))
{}