/**
*	@file Uniform.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Uniform.h"

V3D_NS_BEGIN
VK_NS_BEGIN

Uniform::Uniform( const std::string& name, const int32_t binding, const int32_t offset, const int32_t size, const int32_t glType, const bool writeOnly )
	: name( name )
	, binding( binding )
	, offset( offset )
	, size( size )
	, glValueType( glType )
	, writeOnly( writeOnly )
	//, value()
{}

Uniform::~Uniform() {}

std::string Uniform::getName() const { return name; }

inline int32_t Uniform::getBinding() const { return binding; }

inline int32_t Uniform::getOffset() const { return offset; }

inline int32_t Uniform::getSize() const { return size; }

inline int32_t Uniform::getGLValueType() const { return glValueType; }

inline v3d::vulkan::GLType Uniform::getGLType() const { return v3d::vulkan::toGLTypeEnum( glValueType ); }

bool Uniform::isWriteOnly() const { return writeOnly; }

inline bool Uniform::isFloat() const { return glValueType == 0x1406; }

inline bool Uniform::isVec2() const { return glValueType == 0x8B50; }

inline bool Uniform::isVec3() const { return glValueType == 0x8B51; }

inline bool Uniform::isVec4() const { return glValueType == 0x8B52; }

inline bool Uniform::isInt() const { return glValueType == 0x1404; }

inline bool Uniform::isIVec2() const { return glValueType == 0x8B53; }

inline bool Uniform::isIVec3() const { return glValueType == 0x8B54; }

inline bool Uniform::isIVec4() const { return glValueType == 0x8B55; }

inline bool Uniform::isUint() const { return glValueType == 0x1405; }

inline bool Uniform::isUVec2() const { return glValueType == 0x8DC6; }

inline bool Uniform::isUVec3() const { return glValueType == 0x8DC7; }

inline bool Uniform::isUVec4() const { return glValueType == 0x8DC8; }

inline bool Uniform::isBool() const { return glValueType == 0x8B56; }

inline bool Uniform::isBVec2() const { return glValueType == 0x8B57; }

inline bool Uniform::isBVec3() const { return glValueType == 0x8B58; }

inline bool Uniform::isBVec4() const { return glValueType == 0x8B59; }

inline bool Uniform::isMat2() const { return glValueType == 0x8B5A; }

inline bool Uniform::isMat3() const { return glValueType == 0x8B5B; }

inline bool Uniform::isMat4() const { return glValueType == 0x8B5C; }

inline bool Uniform::isMat2x3() const { return glValueType == 0x8B65; }

inline bool Uniform::isMat2x4() const { return glValueType == 0x8B66; }

inline bool Uniform::isMat3x2() const { return glValueType == 0x8B67; }

inline bool Uniform::isMat3x4() const { return glValueType == 0x8B68; }

inline bool Uniform::isMat4x2() const { return glValueType == 0x8B69; }

inline bool Uniform::isMat4x3() const { return glValueType == 0x8B6A; }

void Uniform::print() const
{
	auto& logger = v3d::Logger::getInstance();
	logger.trace( "[Uniform] info..." );
	logger.trace( "Name: {}", name );
	logger.trace( "Binding: {}", binding );
	logger.trace( "Size: {}", size );
	logger.trace( "Offset: {}", offset );
	logger.trace( "GLType: {} ({})", glValueType, v3d::vulkan::GLDataTypeToString( getGLType() ) );
}

VK_NS_END
V3D_NS_END
