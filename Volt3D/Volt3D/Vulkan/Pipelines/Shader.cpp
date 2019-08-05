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
	, stage( v3d::vulkan::Shader::toShaderStageFlagbits( filePath.filename() ) )
	, filePath( filePath )
{}

Shader::~Shader() {}

bool Shader::init()
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
	//program.dumpReflection();

	// 7. Query all uniforms and attributes

	// uniform blocks
	for(int i = 0 ; i < program.getNumLiveUniformBlocks(); i++)
	{
		const glslang::TObjectReflection& reflection = program.getUniformBlock( i );

		v3d::vulkan::UniformBlockType type = v3d::vulkan::UniformBlockType::eUndefined;
		if (reflection.getType()->getQualifier().storage == glslang::EvqUniform) type = v3d::vulkan::UniformBlockType::eUniform;
		else if (reflection.getType()->getQualifier().storage == glslang::EvqBuffer) type = v3d::vulkan::UniformBlockType::eStorage;
		else if (reflection.getType()->getQualifier().layoutPushConstant) type = v3d::vulkan::UniformBlockType::ePush;

		uniformBlocks.emplace( reflection.getBinding(), std::move( v3d::vulkan::UniformBlock( reflection.name, reflection.getBinding(), reflection.size, type, false ) ) );
	}

	// uniforms
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
				auto uniformBlock = getUniformBlock( split.front() );
				if (uniformBlock.has_value())
				{
					uniformBlock.value().get().uniforms.emplace( reflection.name, std::move( v3d::vulkan::Uniform( reflection.name, reflection.getBinding(), reflection.offset, reflection.size, reflection.glDefineType, false ) ) );
				}
			}
		}
		else
		{
			// Uniforms
			auto& qualifier = reflection.getType()->getQualifier();
			reflection.dump();
			uniforms.emplace( reflection.getBinding(), std::move( v3d::vulkan::Uniform( reflection.name, reflection.getBinding(), reflection.offset, reflection.size, reflection.glDefineType, qualifier.writeonly ) ) );
		}
	}

	for (auto& uniformBlock : uniformBlocks) uniformBlock.second.print( true );
	for (auto& uniform : uniforms) uniform.second.print();

	// vertex attribs
	for (int32_t i{}; i < program.getNumLiveAttributes(); i++)
	{
		auto reflection = program.getPipeInput( i );

		if (reflection.name.empty()) break;
		auto& q = reflection.getType()->getQualifier();
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

void Shader::release()
{
	logicalDevice.destroyShaderModule( shaderModule );
	shaderModule = nullptr;
}

const vk::ShaderModule Shader::get() const
{
	return shaderModule;
}

vk::PipelineShaderStageCreateInfo Shader::getPipelineShaderStageCreateInfo() const
{
	return vk::PipelineShaderStageCreateInfo( vk::PipelineShaderStageCreateFlags(), stage, shaderModule, "main" );
}

std::optional<std::reference_wrapper<v3d::vulkan::UniformBlock>> Shader::getUniformBlock( const uint32_t binding )
{
	auto find_it = uniformBlocks.find( binding );
	if (find_it == uniformBlocks.end()) return std::nullopt;
	return (find_it->second);
}

std::optional<std::reference_wrapper<v3d::vulkan::UniformBlock>> Shader::getUniformBlock( const std::string_view name )
{
	for (auto& uniformBlock : uniformBlocks) if ((uniformBlock.second).name == name) return uniformBlock.second;
	return std::nullopt;
}

vk::ShaderStageFlagBits Shader::getStage() const { return stage; }

std::vector<vk::DescriptorSetLayoutBinding> Shader::getDescriptorSetLayoutBinding() const
{
	std::vector<vk::DescriptorSetLayoutBinding> bindings;

	for (auto& [uniformName, uniformBlock] : uniformBlocks)
	{
		vk::DescriptorType descriptorType;
		if (uniformBlock.getType() == v3d::vulkan::UniformBlockType::eUniform) descriptorType = vk::DescriptorType::eUniformBuffer;
		else if (uniformBlock.getType() == v3d::vulkan::UniformBlockType::eStorage) descriptorType = vk::DescriptorType::eStorageBuffer;
		else continue;
		vk::DescriptorSetLayoutBinding binding
		(
			uniformBlock.binding,
			descriptorType,
			1,
			stage
		);
		bindings.push_back( binding );
	}
	
	for (auto& [uniformName, uniform] : uniforms)
	{
		vk::DescriptorType descriptorType;

		switch (uniform.getGLType())
		{
		case 0x8B5E: // GL_SAMPLER_2D
		case 0x904D: // GL_IMAGE_2D
		case 0x9108: // GL_SAMPLER_2D_MULTISAMPLE
		case 0x9055: // GL_IMAGE_2D_MULTISAMPLE
		{
			descriptorType = uniform.isWriteOnly() ? vk::DescriptorType::eStorageImage : vk::DescriptorType::eCombinedImageSampler;
			vk::DescriptorSetLayoutBinding binding
			(
				uniform.getBinding(),
				descriptorType,
				1,
				stage
			);
			bindings.push_back( binding );
		}
			break;
		case 0x8B60: // GL_SAMPLER_CUBE
		case 0x9050: // GL_IMAGE_CUBE
		default:
			continue;
			break;
		}
	}

	return bindings;
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

const vk::ShaderStageFlagBits Shader::toShaderStageFlagbits( const std::filesystem::path& fileName )
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
