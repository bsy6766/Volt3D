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
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

// Graphics API
#include <vulkan/vulkan.hpp>

// volt3d
#include "Volt3D/Utils/Logger.h"

// cpp
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <filesystem>
#include <sstream>
#include <functional>
#include <limits>
#include <cassert>