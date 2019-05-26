#include <PreCompiled.h>

#include "Shader.h"

#include <fstream>

Shader::Shader( std::string_view fileName, vk::UniqueDevice& device )
	//: shaderModule( device->createShaderModuleUnique( reinterpret_cast<const uint32_t*>(readFile( fileName ).data()) ) )
{}

std::vector<char> Shader::readFile( std::string_view fileName )
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

bool Shader::init( std::string_view fileName, vk::UniqueDevice& device )
{
	auto buffer = readFile( fileName );

	if( buffer.empty() ) return false;

	vk::ShaderModuleCreateInfo createInfo( vk::ShaderModuleCreateFlags(), buffer.size(), reinterpret_cast<const uint32_t*>(buffer.data()) );
	shaderModule = std::move( device->createShaderModuleUnique( createInfo ) );

	return true;
}
