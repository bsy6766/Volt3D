/**
*	@file ShaderModule.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "ShaderModule.h"

#include <fstream>

v3d::vulkan::ShaderModule::ShaderModule()
	: shaderModule()
{}

std::vector<char> v3d::vulkan::ShaderModule::readFile(std::string_view fileName)
{
	std::vector<char> buffer;

	std::ifstream file(fileName.data(), std::ios::ate | std::ios::binary);
	if (!file.is_open()) return buffer;

	std::size_t fileSize = (std::size_t)file.tellg();
	buffer.resize(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

bool v3d::vulkan::ShaderModule::init(std::string_view fileName, const vk::Device& device)
{
	const std::vector<char> buffer = readFile(fileName);
	shaderModule = device.createShaderModuleUnique(vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), buffer.size(), reinterpret_cast<const uint32_t*>(buffer.data())));
	return true;
}
