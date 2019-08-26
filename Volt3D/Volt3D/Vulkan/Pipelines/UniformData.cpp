/**
*	@file UniformData.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "UniformData.h"

V3D_NS_BEGIN
VK_NS_BEGIN

UniformData::UniformData( const std::size_t size )
	: dataPtr( nullptr )
	, size( size )
	, dirty( true )
{
	dataPtr = new unsigned char[size];
}

UniformData::~UniformData() { delete[] dataPtr; }

void UniformData::clear() { memset( dataPtr, 0, size ); }

void UniformData::setData( const void* newDataPtr, const std::size_t newDataSize, std::size_t offset )
{
	if (offset + newDataSize > size) return;
	memcpy( dataPtr + offset, newDataPtr, newDataSize );
	dirty = true;
}

bool UniformData::isDirty() const { return dirty; }

std::size_t UniformData::getSize() const { return size; }

const void* UniformData::getDataPtr() const { return dataPtr; }

VK_NS_END
V3D_NS_END