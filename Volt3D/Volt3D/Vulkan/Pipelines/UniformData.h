/**
*	@file UniformData.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_UNIFORM_DATA_H
#define V3D_VK_UNIFORM_DATA_H

#include <Vulkan/vulkan.hpp>

#include "utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class UniformBlock;

/**
*	@class UniformData
*	@brief A dynamic allocated memory for Uniforms in Vulkan shaders.
*
*	@group Vulkan
*
*	@since 1.0
*/
class UniformData
{
	friend class v3d::vulkan::UniformBlock;
private:
	UniformData() = delete;
	UniformData( const std::size_t size );

	unsigned char* dataPtr;
	std::size_t size;
	bool dirty;

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( UniformData );
	DEFAULT_MOVE_CONSTRUCTORS( UniformData );

	~UniformData();

	void clear();
	void setData( const void* newDataPtr, const std::size_t newDataSize = 0, std::size_t offset = 0 );
	bool isDirty() const;

	std::size_t getSize() const;
	const void* getDataPtr() const;
};

VK_NS_END
V3D_NS_END

#endif