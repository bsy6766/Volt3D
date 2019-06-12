/**
*	@file VertexData.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "VertexData.h"

template<class T>
v3d::VertexData<T>::VertexData()
	: vertices()
{}

template<class T>
std::size_t v3d::VertexData<T>::getSize() const
{
	return vertices.size();
}

template<class T>
std::size_t v3d::VertexData<T>::getDataSize() const
{
	return sizeof(T) * vertices.size();
}

template<class T>
std::vector<T>& v3d::VertexData<T>::getVertexData()
{
	return vertices;
}

template<class T>
const T* v3d::VertexData<T>::getData() const
{
	return vertices.data();
}
