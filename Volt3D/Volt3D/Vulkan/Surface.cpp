#include <PreCompiled.h>

#include "Surface.h"

Surface::Surface( vk::UniqueSurfaceKHR&& surface )
	: surface( std::move( surface ) )
{}
