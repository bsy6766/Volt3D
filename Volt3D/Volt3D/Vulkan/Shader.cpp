/**
*	@file Shader.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Shader.h"

#include <fstream>

v3d::vulkan::Shader::Shader( std::string_view fileName, v3d::vulkan::Device& device )
	//: shaderModule( device->createShaderModuleUnique( reinterpret_cast<const uint32_t*>(readFile( fileName ).data()) ) )
{}

std::vector<char> v3d::vulkan::Shader::readFile( std::string_view fileName )
{
	std::vector<char> buffer;

	std::ifstream file( fileName.data(), std::ios::ate | std::ios::binary );
	if( !file.is_open() ) return buffer;

	std::size_t fileSize = (std::size_t)file.tellg();
	buffer.resize( fileSize );

	file.seekg( 0 );
	file.read( buffer.data(), fileSize );

	file.close();

	return buffer;
}

bool v3d::vulkan::Shader::init( std::string_view fileName, v3d::vulkan::Device& device )
{
	auto buffer = readFile( fileName );

	if( buffer.empty() ) return false;

	vk::ShaderModuleCreateInfo createInfo( vk::ShaderModuleCreateFlags(), buffer.size(), reinterpret_cast<const uint32_t*>(buffer.data()) );
	shaderModule = std::move( device->createShaderModuleUnique( createInfo ) );

	return true;
}
