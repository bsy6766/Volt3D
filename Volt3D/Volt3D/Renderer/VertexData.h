/**
*	@file VertexData.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VERTEX_DATA_BUFFER_H
#define V3D_VERTEX_DATA_BUFFER_H

#include <vulkan/vulkan.hpp>

#include <vector>

#include "Utils/Macros.h"

namespace v3d
{
	/**
	*	@class VertexData
	*	@brief VertexData
	*
	*	@since 1.0
	*/
	template<class T>
	class VOLT3D_DLL VertexData
	{
		friend class Context;

	private:
		std::vector<T> vertices;

	public:
		VertexData();
		DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(VertexData);
		DEFAULT_MOVE_CONSTRUCTORS(VertexData);
		~VertexData() {};

		std::size_t getSize() const;
		std::size_t getDataSize() const;
		std::vector<T>& getVertexData();
		const T* getData() const;
	};
}

#endif