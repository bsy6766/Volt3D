/**
*	@file ShaderModule.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_SHADER_H
#define V3D_VK_SHADER_H

#include <vulkan/vulkan.hpp>

#include <string_view>

#include "utils/Macros.h"

namespace v3d
{
	namespace vulkan
	{
		class Device;

		/**
		*	@class ShaderModule
		*	@brief Wrapper for Vulkan's shader module
		*
		*	@group Vulkan
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL ShaderModule
		{
			friend class Context;
			friend class Pipeline;

		private:
			ShaderModule();

			vk::UniqueShaderModule shaderModule;

			std::vector<char> readFile(std::string_view fileName);

			bool init(std::string_view fileName, const v3d::vulkan::Device& device);

		public:
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(ShaderModule);
			DEFAULT_MOVE_CONSTRUCTORS(ShaderModule);
			~ShaderModule() {}

			UNIQUE_TO_CPP_VK_HANDLE(vk::UniqueShaderModule, vk::ShaderModule, shaderModule);
		};
	}
}

#endif