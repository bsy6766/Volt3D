/**
*	@file ProjectionType.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_PROJECTION_TYPE_H
#define V3D_PROJECTION_TYPE_H

#include <string>

namespace v3d
{
	/**
	*	@enum ProjectionType
	*	@brief Represent 2 different camera projection type in OpenGL.
	*/
	enum class ProjectionType : bool
	{
		ePerspective = 0,
		eOrthographic,
	};

	inline std::string projectionTypeToString(v3d::ProjectionType value)
	{
		switch (value)
		{
		case v3d::ProjectionType::ePerspective: return "Perspective";
		case v3d::ProjectionType::eOrthographic: return "Orthographic";
		default: return "Invalid projection type";
		}
	}
}

#endif