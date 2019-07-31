/**
*	@file Shader.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Shader.h"

#include <StandAlone/ResourceLimits.h>

#include "Vulkan/Context.h"
#include "Vulkan/Devices/LogicalDevice.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Shader::Shader( const std::filesystem::path& filePath )
	: logicalDevice( v3d::vulkan::Context::get()->getLogicalDevice()->get() )
	, shaderModule( nullptr )
	, stage()
{}

EShLanguage Shader::getEShLanguage() const
{
	switch (stage)
	{
	case vk::ShaderStageFlagBits::eVertex: return EShLanguage::EShLangVertex;
	case vk::ShaderStageFlagBits::eTessellationControl: return EShLanguage::EShLangTessControl;
	case vk::ShaderStageFlagBits::eTessellationEvaluation: return EShLanguage::EShLangTessEvaluation;
	case vk::ShaderStageFlagBits::eGeometry: return EShLanguage::EShLangGeometry;
	case vk::ShaderStageFlagBits::eFragment: return EShLanguage::EShLangFragment;
	case vk::ShaderStageFlagBits::eCompute: return EShLanguage::EShLangCompute;
	case vk::ShaderStageFlagBits::eRaygenNV: return EShLanguage::EShLangRayGenNV;
	case vk::ShaderStageFlagBits::eAnyHitNV: return EShLanguage::EShLangAnyHitNV;
	case vk::ShaderStageFlagBits::eClosestHitNV: return EShLanguage::EShLangClosestHitNV;
	case vk::ShaderStageFlagBits::eMissNV: return EShLanguage::EShLangMissNV;
	case vk::ShaderStageFlagBits::eIntersectionNV: return EShLanguage::EShLangIntersectNV;
	case vk::ShaderStageFlagBits::eCallableNV: return EShLanguage::EShLangCallableNV;
	case vk::ShaderStageFlagBits::eTaskNV: return EShLanguage::EShLangTaskNV;
	case vk::ShaderStageFlagBits::eMeshNV: return EShLanguage::EShLangMeshNV;
	default: break;
	}

	return EShLanguage::EShLangCount;
}

bool Shader::init( const std::filesystem::path& filePath )
{
	EShLanguage language = getEShLanguage();
	if (language == EShLanguage::EShLangCount) return false;

	glslang::TProgram program;
	glslang::TShader shader( language );
	TBuiltInResource resource = glslang::DefaultTBuiltInResource;

	std::vector<char> shaderSource = readFile( filePath );
	if (shaderSource.empty()) return false;
	int shaderSourceLength = static_cast<int>(shaderSource.size());
	std::string shaderSourceStr( shaderSource.begin(), shaderSource.end() );
	const char* shaderSourceCStr = shaderSourceStr.c_str();

	const char* shaderFileName = filePath.filename().string().c_str();
	
	shader.setStringsWithLengthsAndNames( &shaderSourceCStr, &shaderSourceLength, &shaderFileName, 1 );

	shader.setEnvInput( glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 110 );
	shader.setEnvClient( glslang::EShClientVulkan, glslang::EShTargetVulkan_1_1 );
	shader.setEnvTarget( glslang::EShTargetSpv, glslang::EShTargetSpv_1_4 );

	glslang::EShTargetClientVersion defaultVersion = glslang::EShTargetVulkan_1_1;
	EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);

	if (!shader.parse( &resource, defaultVersion, false, messages )) return false;

	//https://github.com/qnope/GameEngine/blob/334e2fed2e43a007fc90f0aa70c9858983c5b4e5/Moteur/Vulkan/ShaderCompiler.cpp


	return true;
}

std::vector<char> Shader::readFile( const std::filesystem::path& filePath )
{
	std::vector<char> buffer;

	std::ifstream file( fileName, std::ios::ate | std::ios::binary );
	if (!file.is_open()) return buffer;

	std::size_t fileSize = (std::size_t)file.tellg();
	buffer.resize( fileSize );

	file.seekg( 0 );
	file.read( buffer.data(), fileSize );

	file.close();

	return buffer;
}

const vk::ShaderStageFlagBits Shader::getShaderStage( const std::filesystem::path& fileName )
{
	if (fileName.has_extension())
	{
		const auto ext = fileName.extension().string();
		if (ext == "vert") return vk::ShaderStageFlagBits::eVertex;
		if (ext == "tesc") return vk::ShaderStageFlagBits::eTessellationControl;
		if (ext == "tese") return vk::ShaderStageFlagBits::eTessellationEvaluation;
		if (ext == "geom") return vk::ShaderStageFlagBits::eGeometry;
		if (ext == "frag") return vk::ShaderStageFlagBits::eFragment;
		if (ext == "comp") return vk::ShaderStageFlagBits::eCompute;
		if (ext == "mesh") return vk::ShaderStageFlagBits::eMeshNV;
		if (ext == "task") return vk::ShaderStageFlagBits::eTaskNV;
		if (ext == "rgen") return vk::ShaderStageFlagBits::eRaygenNV;
		if (ext == "rint") return vk::ShaderStageFlagBits::eIntersectionNV;
		if (ext == "rahit") return vk::ShaderStageFlagBits::eAnyHitNV;
		if (ext == "rchit") return vk::ShaderStageFlagBits::eClosestHitNV;
		if (ext == "rmiss") return vk::ShaderStageFlagBits::eMissNV;
		if (ext == "rcall") return vk::ShaderStageFlagBits::eCallableNV;
	}

	return vk::ShaderStageFlagBits::eAll;
}

VK_NS_END
V3D_NS_END
