#pragma once

#include <string>
#include <vulkan/vulkan.hpp>

#include "utils/Macros.h"

class Shader
{
private:
	
	vk::UniqueShaderModule shaderModule;

	std::vector<char> readFile( std::string_view fileName );

	bool init( std::string_view fileName, vk::UniqueDevice& device );
public:
	Shader( std::string_view fileName, vk::UniqueDevice& device );

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Shader );
	DEFAULT_MOVE_CONSTRUCTORS( Shader );
	~Shader() {}

	const vk::UniqueShaderModule& operator()() const noexcept { return shaderModule; }
};

