#pragma

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

class Surface;
class PhysicalDevice;

class Device
{
	friend class Vulkan;
private:
	Device();

	vk::UniqueDevice device;
	uint32_t graphicsQueueFamilyIndex;
	uint32_t presentQueueFamilyIndex;

	bool init( PhysicalDevice& physicalDevice, Surface& surface );

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Device );
	DEFAULT_MOVE_CONSTRUCTORS( Device );
	~Device() {}

	uint32_t getGraphicsQueueFamilyIndex() const
	{ return graphicsQueueFamilyIndex; }

	uint32_t getPresentQueueFamilyIndex() const
	{ return presentQueueFamilyIndex; }
};