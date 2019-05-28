/**
*	@file Shader.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <vulkan/vulkan.hpp>

#include <string_view>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class Device;

		/**
		*	@class Shader
		*	@brief Wrapper for Vulkan's shader module
		*
		*	@group Vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Shader
		{
		private:
			vk::UniqueShaderModule shaderModule;

			std::vector<char> readFile(std::string_view fileName);

		public:
			Shader(std::string_view fileName, v3d::vulkan::Device& device);
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Shader);
			DEFAULT_MOVE_CONSTRUCTORS(Shader);
			~Shader() {}

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueShaderModule, vk::ShaderModule, shaderModule);
		};
	}
}
