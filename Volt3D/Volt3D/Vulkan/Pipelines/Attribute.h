/**
*	@file Attribute.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_ATTRIBUTE_H
#define V3D_VK_ATTRIBUTE_H

#include <vulkan/vulkan.hpp>

#include <string>

#include "utils/Macros.h"

#include "GLType.h"

V3D_NS_BEGIN
VK_NS_BEGIN

class ShaderState;

/**
*	@class Attribute
*	@brief A class that contains shader attributes information
*
*	@group Vulkan
*
*	@since 1.0
*/
class VOLT3D_DLL Attribute
{
	friend class v3d::vulkan::ShaderState;

private:
	Attribute() = delete;
	Attribute( const std::string& name, const int32_t set, const int32_t location, const int32_t size, const int32_t glType );

	std::string name;

	int32_t set;
	int32_t location;
	int32_t size;
	int32_t glValueType;

public:
	~Attribute();

	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Attribute );
	DEFAULT_MOVE_CONSTRUCTORS( Attribute );

	/** Get name of this attribute in shader */
	std::string getName() const;

	/** Get set of this attribute in shader */
	int32_t getSet() const;

	/** Get location of this attribute in shader */
	int32_t getLocation() const;

	/** Get size of this attribute */
	int32_t getSize() const;

	/** Get GL type of this attribute */
	int32_t getGlValueType() const;

	/** Get data type */
	v3d::vulkan::GLType getGLType() const;

	/** Log Attribute */
	void print() const;
};

VK_NS_END
V3D_NS_END

#endif