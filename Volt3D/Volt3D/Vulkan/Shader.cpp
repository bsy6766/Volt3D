/**
*	@file Shader.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Shader.h"

#include <fstream>

#include "Device.h"

v3d::vulkan::Shader::Shader()
	: shaderModule()
{}

std::vector<char> v3d::vulkan::Shader::readFile(std::string_view fileName)
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

bool v3d::vulkan::Shader::init(std::string_view fileName, v3d::vulkan::Device& device)
{
	shaderModule = std::move(device.createShaderModuleUnique(readFile(fileName)));
	return true;
}
