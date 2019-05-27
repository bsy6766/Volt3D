/**
*	@file Surface.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Surface.h"

v3d::vulkan::Surface::Surface( vk::UniqueSurfaceKHR&& surface )
	: surface( std::move( surface ) )
{}
