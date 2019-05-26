#pragma

#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

class PhysicalDevice;
class Device;
class Surface;

class SwapChain
{
private:
	vk::UniqueSwapchainKHR swapChain;
	std::vector<vk::UniqueImageView> imageViews;

public:
	SwapChain();
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( SwapChain );
	DEFAULT_MOVE_CONSTRUCTORS( SwapChain );
	~SwapChain() {};

	bool init( PhysicalDevice& physicalDevice, Device& device, Surface& surface );
};