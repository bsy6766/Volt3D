#pragma

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

class Surface
{
private:
	vk::UniqueSurfaceKHR surface;

public:
	Surface( vk::UniqueSurfaceKHR&& surface );
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Surface );
	DEFAULT_MOVE_CONSTRUCTORS( Surface );
	~Surface() {}

	explicit operator const vk::UniqueSurfaceKHR& () const noexcept { return surface; }
	vk::UniqueSurfaceKHR& getHandle() { return surface; }
};