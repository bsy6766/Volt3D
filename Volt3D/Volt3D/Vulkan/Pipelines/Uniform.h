/**
*	@file Uniform.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_UNIFORM_H
#define V3D_VK_UNIFORM_H

#include <Vulkan/vulkan.hpp>

#include "GLType.h"
#include "utils/Macros.h"

#include <vector>
#include <optional>

V3D_NS_BEGIN
class ShaderState;

VK_NS_BEGIN

class VOLT3D_DLL Uniform
{
	friend class v3d::ShaderState;
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
	Uniform() = delete;
	Uniform( const std::string& name, const int32_t binding, const int32_t offset, const int32_t size, const int32_t glType, const bool writeOnly );

	std::string name;
	//v3d::vulkan::Uniform::Value value;

	int32_t binding;
	int32_t offset;
	int32_t size;
	int32_t glValueType;

	// @todo readonly?
	bool writeOnly;

public:
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Uniform );
	DEFAULT_MOVE_CONSTRUCTORS( Uniform );

	~Uniform();

	/** Get name of uniform */
	inline std::string getName() const;

	/** Get binding of uniform */
	inline int32_t getBinding() const;

	/** Get offset of uniform from uniform block*/
	inline int32_t getOffset() const;

	/** Get size of this uniform */
	inline int32_t getSize() const;

	/** Get gl value type*/
	inline int32_t getGLValueType() const;

	/** Get data type */
	inline v3d::vulkan::GLType getGLType() const;

	/** Check if this uniform block is write only */
	bool isWriteOnly() const;

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