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
#include "Shader/ShaderState.h"
#include "Vulkan/Utils.h"

V3D_NS_BEGIN
VK_NS_BEGIN

ShaderModule::ShaderModule()
	: shaderModule( nullptr )
	, stage()
{}

ShaderModule::~ShaderModule()
{
	v3d::vulkan::LogicalDevice::get()->getVKLogicalDevice().destroyShaderModule( shaderModule );
}

bool ShaderModule::compile( const std::vector<char>& sourceCode, const std::filesystem::path& filePath, v3d::ShaderState& shaderState )
{
	EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);

	// set stage
	stage = v3d::vulkan::utils::extToShaderStageFlagBit( filePath.filename() );
	if (stage == vk::ShaderStageFlagBits::eAll) return false;
		
	// 1. Generate c strings...
	int shaderSourceLength = static_cast<int>(sourceCode.size());
	std::string shaderSourceStr( sourceCode.begin(), sourceCode.end() );
	const char* shaderSourceCStr = shaderSourceStr.c_str();
	std::string shaderFileName = filePath.filename().string();
	const char* shaderFileNameCStr = shaderFileName.c_str();

	// 2. Get language
	EShLanguage language;

	switch (stage)
	{
	case vk::ShaderStageFlagBits::eVertex: language = EShLanguage::EShLangVertex;
	case vk::ShaderStageFlagBits::eTessellationControl: language = EShLanguage::EShLangTessControl;
	case vk::ShaderStageFlagBits::eTessellationEvaluation: language = EShLanguage::EShLangTessEvaluation;
	case vk::ShaderStageFlagBits::eGeometry: language = EShLanguage::EShLangGeometry;
	case vk::ShaderStageFlagBits::eFragment: language = EShLanguage::EShLangFragment;
	case vk::ShaderStageFlagBits::eCompute: language = EShLanguage::EShLangCompute;
	case vk::ShaderStageFlagBits::eRaygenNV: language = EShLanguage::EShLangRayGenNV;
	case vk::ShaderStageFlagBits::eAnyHitNV: language = EShLanguage::EShLangAnyHitNV;
	case vk::ShaderStageFlagBits::eClosestHitNV: language = EShLanguage::EShLangClosestHitNV;
	case vk::ShaderStageFlagBits::eMissNV: language = EShLanguage::EShLangMissNV;
	case vk::ShaderStageFlagBits::eIntersectionNV: language = EShLanguage::EShLangIntersectNV;
	case vk::ShaderStageFlagBits::eCallableNV: language = EShLanguage::EShLangCallableNV;
	case vk::ShaderStageFlagBits::eTaskNV: language = EShLanguage::EShLangTaskNV;
	case vk::ShaderStageFlagBits::eMeshNV: language = EShLanguage::EShLangMeshNV;
	default: return false;
	}
	
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
	shaderState.init( program );

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

const vk::ShaderModule ShaderModule::getShaderModule() const 
{ 
	return shaderModule; 
}

inline vk::PipelineShaderStageCreateInfo ShaderModule::getPipelineShaderStageCreateInfo() const
{
	return vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), stage, shaderModule, "main" );
}

inline vk::ShaderStageFlagBits ShaderModule::getStage() const
{ 
	return stage; 
}

VK_NS_END
V3D_NS_END
