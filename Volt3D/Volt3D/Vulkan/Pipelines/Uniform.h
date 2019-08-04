/**
*	@file Uniform.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_UNIFORM_H
#define V3D_VK_UNIFORM_H

#include <Vulkan/vulkan.hpp>

#include "UniformType.h"
#include "utils/Macros.h"

#include <vector>
#include <optional>

V3D_NS_BEGIN
VK_NS_BEGIN

class VOLT3D_DLL Uniform
{
	friend class Shader;
//
//public:
//	union Value
//	{
//	public:
//		bool boolValue;
//		int intValue;
//		float floatValue;
//		float v2Value[2];
//		float v3Value[3];
//		float v4Value[4];
//		float mat4Value[16];
//	};

private:
	std::string name;
	//v3d::vulkan::Uniform::Value value;

	// Uniform binding is always -1
	//int32_t binding;

	int32_t offset;
	int32_t size;
	int32_t glType;

public:
	Uniform() = delete;
	Uniform( const std::string& name );
	~Uniform();

	/** Get name of uniform */
	inline std::string getName() const;

	/** Get binding of uniform */
	//inline int32_t getBinding() const;

	/** Get offset of uniform from uniform block*/
	inline int32_t getOffset() const;

	/** Get size of this uniform */
	inline int32_t getSize() const;

	/** Get GL type value */
	inline int32_t getGLType() const;

	/** Get data type */
	inline v3d::vulkan::UniformType getType() const;

	/** Check if data type is float */
	inline bool isFloat() const;

	/** Check if data type is vec2 */
	inline bool isVec2() const;

	/** Check if data type is vec3 */
	inline bool isVec3() const;

	/** Check if data type is vec4 */
	inline bool isVec4() const;

	/** Check if data type is int */
	inline bool isInt() const;

	/** Check if data type is iVec2 */
	inline bool isIVec2() const;

	/** Check if data type is iVec3 */
	inline bool isIVec3() const;

	/** Check if data type is iVec4 */
	inline bool isIVec4() const;

	/** Check if data type is uint */
	inline bool isUint() const;

	/** Check if data type is uVec2 */
	inline bool isUVec2() const;

	/** Check if data type is uVec3 */
	inline bool isUVec3() const;

	/** Check if data type is uVec4 */
	inline bool isUVec4() const;

	/** Check if data type is bool */
	inline bool isBool() const;

	/** Check if data type is bVec2 */
	inline bool isBVec2() const;

	/** Check if data type is bVec3 */
	inline bool isBVec3() const;

	/** Check if data type is bVec4 */
	inline bool isBVec4() const;

	/** Check if data type is mat2 */
	inline bool isMat2() const;

	/** Check if data type is mat3 */
	inline bool isMat3() const;

	/** Check if data type is mat4 */
	inline bool isMat4() const;

	/** Check if data type is mat2x3 */
	inline bool isMat2x3() const;

	/** Check if data type is mat2x4 */
	inline bool isMat2x4() const;

	/** Check if data type is mat3x2 */
	inline bool isMat3x2() const;

	/** Check if data type is mat3x4 */
	inline bool isMat3x4() const;

	/** Check if data type is mat4x2 */
	inline bool isMat4x2() const;

	/** Check if data type is mat4x3 */
	inline bool isMat4x3() const;

	/** Log uniform */
	void print() const;
};

VK_NS_END
V3D_NS_END

#endif