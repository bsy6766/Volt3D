/**
*	@file UniformValue.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_UNIFORM_VALUE_H
#define V3D_VK_UNIFORM_VALUE_H

#include "Utils/Macros.h"

V3D_NS_BEGIN
VK_NS_BEGIN

/**
*	@class UniformValue
*	@brief A value that can hold bool to mat4
*/
class VOLT3D_DLL UniformValue
{
private:
	UniformValue();

public:
	~UniformValue();

	union Value
	{
	public:
		bool boolValue;
		int intValue;
		float floatValue;
		float v2Value[2];
		float v3Value[3];
		float v4Value[4];
		float mat4Value[16];
	} value;
};

VK_NS_END
V3D_NS_END

#endif