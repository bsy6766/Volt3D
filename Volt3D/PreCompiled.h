/**
*	@file PreCompiled.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// externals
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <nlohmann/json.hpp>

// Graphics API
#include <vulkan/vulkan.hpp>

// volt3d
#include "Volt3D/Utils/Logger.h"

// cpp
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <filesystem>
#include <sstream>
#include <functional>
#include <limits>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <typeindex>