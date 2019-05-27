#include <PreCompiled.h>

#include "Surface.h"

v3d::vulkan::Surface::Surface( vk::UniqueSurfaceKHR&& surface )
	: surface( std::move( surface ) )
{}
