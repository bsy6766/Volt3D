/**
*	@file Shader.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Shader.h"

#include <SPIRV/GlslangToSpv.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/Include/ResourceLimits.h>

#include "Vulkan/Context.h"
#include "Vulkan/Devices/LogicalDevice.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Shader::Shader( const std::filesystem::path& filePath )
	: logicalDevice( v3d::vulkan::Context::get()->getLogicalDevice()->get() )
	, shaderModule( nullptr )
	, stage()
{
	EShLanguage language = getEShLanguage();
	//if (language == EShLanguage::EShLangCount) return false;

	glslang::TProgram program;
	glslang::TShader shader( language );
	TBuiltInResource resource = DefaultTBuiltInResource;
}

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
