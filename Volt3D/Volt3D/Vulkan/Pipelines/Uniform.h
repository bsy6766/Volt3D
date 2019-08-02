/**
*	@file Uniform.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_UNIFORM_H
#define V3D_VK_UNIFORM_H

#include "utils/Macros.h"

#include <Vulkan/vulkan.hpp>

#include <vector>
#include <optional>

V3D_NS_BEGIN
VK_NS_BEGIN

class VOLT3D_DLL Uniform
{
	friend class Shader;

public:
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
	};

	enum class Type
	{
		eUndefined = 0,

		eFloat = 0x1406,
		eVec2 = 0x8B50,
		eVec3 = 0x8B51,
		eVec4 = 0x8B52,

		eInt = 0x1404,
		eIVec2 = 0x8B53,
		eIVec3 = 0x8B54,
		eIVec4 = 0x8B55,

		eUint = 0x1405,
		eUVec2 = 0x8DC6,
		eUVec3 = 0x8DC7,
		eUVec4 = 0x8DC8,

		eBool = 0x8B56,
		eBVec2 = 0x8B57,
		eBVec3 = 0x8B58,
		eBVec4 = 0x8B59,

		eMat2 = 0x8B5A,
		eMat3 = 0x8B5B,
		eMat4 = 0x8B5C,
		eMat2x3 = 0x8B65,
		eMat2x4 = 0x8B66,
		eMat3x2 = 0x8B67,
		eMat3x4 = 0x8B68,
		eMat4x2 = 0x8B69,
		eMat4x3 = 0x8B6A,
	};

private:
	std::string name;
	v3d::vulkan::Uniform::Value value;

	int32_t binding;
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
	inline int32_t getBinding() const;

	/** Get offset of uniform from uniform block*/
	inline int32_t getOffset() const;

	/** Get data size of this uniform */
	inline int32_t getSize() const;

	/** Get GL type value */
	inline int32_t getGLType() const;

	/** Get data type */
	inline v3d::vulkan::Uniform::Type getDataType() const;

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


};

VK_NS_END
V3D_NS_END

#endif