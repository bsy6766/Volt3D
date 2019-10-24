/**
*	@file ShaderModule.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "ShaderModule.h"

#include <SPIRV/GlslangToSpv.h>
#include <StandAlone/ResourceLimits.h>
#include <StandAlone/DirStackFileIncluder.h>

#include "Vulkan/Devices/LogicalDevice.h"

V3D_NS_BEGIN
VK_NS_BEGIN

ShaderModule::ShaderModule()
	: shaderModule( nullptr )
{}

ShaderModule::~ShaderModule()
{
	v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice().destroyShaderModule( shaderModule );
}

bool ShaderModule::compile( const std::vector<char>& sourceCode )
{
	if (sourceCode.empty()) 
		return false;

	EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);

	// 0. Read Shader file
	//std::vector<char> shaderCode = v3d::FileSystem::readShaderFile( filePath.string().c_str() );

	// 1. Generate c strings...
	int shaderSourceLength = static_cast<int>(sourceCode.size());
	std::string shaderSourceStr( sourceCode.begin(), sourceCode.end() );
	const char* shaderSourceCStr = shaderSourceStr.c_str();
	std::string shaderFileName = filePath.filename().string();
	const char* shaderFileNameCStr = shaderFileName.c_str();

	// 2. Get language
	EShLanguage language = getEShLanguage();
	if (language == EShLanguage::EShLangCount) return false;

	// 3. Create glslang shader
	TBuiltInResource resource = glslang::DefaultTBuiltInResource;
	const glslang::EShTargetClientVersion defaultVersion = glslang::EShTargetVulkan_1_1;
	const glslang::EShClient client = glslang::EShClientVulkan;

	glslang::TShader shader( language );
	shader.setStringsWithLengthsAndNames( &shaderSourceCStr, &shaderSourceLength, &shaderFileNameCStr, 1 );
	shader.setEnvInput( glslang::EShSourceGlsl, language, client, 110 );
	shader.setEnvClient( client, defaultVersion );
	shader.setEnvTarget( glslang::EShTargetSpv, glslang::EShTargetSpv_1_3 );

	// 4. Parse the shader
	if (!shader.parse( &resource, defaultVersion, false, messages ))
	{
		v3d::Logger::getInstance().error( "SPIRV failed to parse shader: " + filePath.string() );
		return false;
	}

	DirStackFileIncluder includer;
	std::string preprocessedGLSL;
	shader.preprocess( &resource, defaultVersion, ENoProfile, false, false, messages, &preprocessedGLSL, includer );

	v3d::Logger::getInstance().trace( shader.getInfoLog() );
	v3d::Logger::getInstance().trace( shader.getInfoDebugLog() );

	// 5. Create program and add & link shader
	glslang::TProgram program;
	program.addShader( &shader );
	if (!program.link( messages ))
	{
		v3d::Logger::getInstance().error( "glslang failed to link shader" );
		return false;
	}

	// 6. Build reflection
	program.buildReflection();
	//program.dumpReflection();

	// 7. Query all uniforms and attributes
	shaderState->init( program );

	// 8. Create spirv file
	glslang::SpvOptions spvOptions;
#ifdef BUILD_DEBUG
	spvOptions.generateDebugInfo = true;
	spvOptions.disableOptimizer = true;
	spvOptions.optimizeSize = false;
#else
	spvOptions.generateDebugInfo = false;
	spvOptions.disableOptimizer = false;
	spvOptions.optimizeSize = true;
#endif

	spv::SpvBuildLogger logger;
	std::vector<uint32_t> spirv;
	GlslangToSpv( *program.getIntermediate( static_cast<EShLanguage>(language) ), spirv, &logger, &spvOptions );

	// 9. Create shader module
	vk::ShaderModuleCreateInfo createInfo( vk::ShaderModuleCreateFlags(), spirv.size() * sizeof( uint32_t ), spirv.data() );
	shaderModule = v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice().createShaderModule( createInfo );

	// Done.
	return true;
}

vk::ShaderModule ShaderModule::get() const { return shaderModule; }

VK_NS_END
V3D_NS_END