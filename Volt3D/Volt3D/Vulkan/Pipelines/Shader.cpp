/**
*	@file Shader.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Shader.h"

#include <SPIRV/GlslangToSpv.h>
#include <StandAlone/ResourceLimits.h>
#include <StandAlone/DirStackFileIncluder.h>

#include "Vulkan/Context.h"
#include "Vulkan/Devices/LogicalDevice.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Shader::Shader( const std::filesystem::path& filePath )
	: logicalDevice( v3d::vulkan::Context::get()->getLogicalDevice()->get() )
	, shaderModule( nullptr )
	, stage( v3d::vulkan::Shader::getShaderStage( filePath.filename() ) )
	, filePath( filePath )
{}

Shader::~Shader()
{
	logicalDevice.destroyShaderModule( shaderModule );
}

bool Shader::init( const std::filesystem::path& filePath )
{
	EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);

	// 0. Read Shader file
	std::vector<char> shaderSource = readFile( filePath );
	if (shaderSource.empty()) return false;
	
	// 1. Generate c strings...
	int shaderSourceLength = static_cast<int>(shaderSource.size());
	std::string shaderSourceStr( shaderSource.begin(), shaderSource.end() );
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
	program.dumpReflection();

	// uniform blocks
	for (int32_t i{ program.getNumLiveUniformBlocks() - 1 }; i >= 0; i--)
	{
		const glslang::TObjectReflection& reflection = program.getUniformBlock( i );
		reflection.dump();
		//reflection.index

		/*
		for (auto& [uniformBlockName, uniformBlock] : m_uniformBlocks)
		{
			if (uniformBlockName == reflection.name)
			{
				uniformBlock.m_stageFlags |= stageFlag;
				return;
			}
		}

		auto type{ UniformBlock::Type::None };

		if (reflection.getType()->getQualifier().storage == glslang::EvqUniform)
		{
			type = UniformBlock::Type::Uniform;
		}

		if (reflection.getType()->getQualifier().storage == glslang::EvqBuffer)
		{
			type = UniformBlock::Type::Storage;
		}

		if (reflection.getType()->getQualifier().layoutPushConstant)
		{
			type = UniformBlock::Type::Push;
		}

		m_uniformBlocks.emplace( reflection.name, UniformBlock( reflection.getBinding(), reflection.size, stageFlag, type ) );
		*/
	}


	// uniforms
	auto i2 = program.getNumUniformBlocks();
	auto i3 = program.getNumLiveUniformVariables();
	for (int32_t i = 0; i < program.getNumLiveUniformVariables(); i++)
	{
		const glslang::TObjectReflection& reflection = program.getUniform( i );

		if (reflection.getBinding() == -1)
		{
			std::vector<std::string> split;
			std::stringstream ss( reflection.name );

			while (ss.good())
			{
				std::string substr;
				std::getline( ss, substr, '.' );
				split.push_back( substr );
			}

			if (split.size() > 1)
			{
				reflection.dump();

				//for (auto& [uniformBlockName, uniformBlock] : m_uniformBlocks)
				//{
				//	if (uniformBlockName == splitName.at( 0 ))
				//	{
				//		uniformBlock.m_uniforms.emplace( String::ReplaceFirst( reflection.name, splitName.at( 0 ) + ".", "" ),
				//			Uniform( reflection.getBinding(), reflection.offset, ComputeSize( reflection.getType() ), reflection.glDefineType, false, false,
				//				stageFlag ) );
				//		return;
				//	}
				//}
			}
		}

		//for (auto& [uniformName, uniform] : m_uniforms)
		//{
		//	if (uniformName == reflection.name)
		//	{
		//		uniform.m_stageFlags |= stageFlag;
		//		return;
		//	}
		//}

		//auto& qualifier{ reflection.getType()->getQualifier() };
		//m_uniforms.emplace( reflection.name, Uniform( reflection.getBinding(), reflection.offset, -1, reflection.glDefineType, qualifier.readonly, qualifier.writeonly, stageFlag ) );


	}

	// vertex attribs

	for (int32_t i{}; i < program.getNumLiveAttributes(); i++)
	{
		auto reflection = program.getPipeInput( i );

		if (reflection.name.empty()) break;
		reflection.dump();

		/*
		for (const auto& [attributeName, attribute] : m_attributes)
		{
			if (attributeName == reflection.name)
			{
				return;
			}
		}

		auto& qualifier{ reflection.getType()->getQualifier() };
		m_attributes.emplace( reflection.name, Attribute( qualifier.layoutSet, qualifier.layoutLocation, ComputeSize( reflection.getType() ), reflection.glDefineType ) );

		*/
	}

	// 7. Query all uniforms and attributes
	/*
	for (uint32_t dim{}; dim < 3; ++dim)
	{
		auto localSize{ program.getLocalSize( dim ) };

		if (localSize > 1)
		{
			m_localSizes[dim] = localSize;
		}
	}

	for (int32_t i{ program.getNumLiveUniformBlocks() - 1 }; i >= 0; i--)
	{
		LoadUniformBlock( program, moduleFlag, i );
	}

	for (int32_t i{}; i < program.getNumLiveUniformVariables(); i++)
	{
		LoadUniform( program, moduleFlag, i );
	}

	for (int32_t i{}; i < program.getNumLiveAttributes(); i++)
	{
		LoadVertexAttribute( program, moduleFlag, i );
	}
	*/

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
	shaderModule = logicalDevice.createShaderModule( createInfo );

	// Done.
	return true;
}

const vk::ShaderModule Shader::get() const
{
	return shaderModule;
}

vk::PipelineShaderStageCreateInfo Shader::getPipelineShaderStageCreateInfo() const
{
	return vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), stage, shaderModule, "main" );
}

std::optional<v3d::vulkan::UniformBlock> Shader::getUniformBlock( const uint32_t binding )
{
	auto find_it = uniformBlocks.find( binding );
	if (find_it == uniformBlocks.end()) return std::nullopt;
	return find_it->second;
}

std::optional<v3d::vulkan::UniformBlock> Shader::getUniformBlock( const std::string_view name )
{
	for (auto& ub : uniformBlocks) if ((ub.second).name == name) return ub.second;
	return std::nullopt;
}

std::vector<char> Shader::readFile( const std::filesystem::path& filePath )
{
	std::vector<char> buffer;

	std::ifstream file( filePath, std::ios::ate | std::ios::binary );
	if (!file.is_open()) return buffer;

	std::size_t fileSize = (std::size_t)file.tellg();
	buffer.resize( fileSize );

	file.seekg( 0 );
	file.read( buffer.data(), fileSize );

	file.close();

	return buffer;
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
		if (ext == ".vert") return vk::ShaderStageFlagBits::eVertex;
		if (ext == ".tesc") return vk::ShaderStageFlagBits::eTessellationControl;
		if (ext == ".tese") return vk::ShaderStageFlagBits::eTessellationEvaluation;
		if (ext == ".geom") return vk::ShaderStageFlagBits::eGeometry;
		if (ext == ".frag") return vk::ShaderStageFlagBits::eFragment;
		if (ext == ".comp") return vk::ShaderStageFlagBits::eCompute;
		if (ext == ".mesh") return vk::ShaderStageFlagBits::eMeshNV;
		if (ext == ".task") return vk::ShaderStageFlagBits::eTaskNV;
		if (ext == ".rgen") return vk::ShaderStageFlagBits::eRaygenNV;
		if (ext == ".rint") return vk::ShaderStageFlagBits::eIntersectionNV;
		if (ext == ".rahit") return vk::ShaderStageFlagBits::eAnyHitNV;
		if (ext == ".rchit") return vk::ShaderStageFlagBits::eClosestHitNV;
		if (ext == ".rmiss") return vk::ShaderStageFlagBits::eMissNV;
		if (ext == ".rcall") return vk::ShaderStageFlagBits::eCallableNV;
	}

	return vk::ShaderStageFlagBits::eAll;
}

VK_NS_END
V3D_NS_END
