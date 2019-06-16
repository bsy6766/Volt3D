#ifndef V3DPIVOT_H
#define V3DPIVOT_H

#include <string>

namespace v3d
{
	/**
	*	@enum Pivot
	*	Defines commonly used pivot positions in 3D coordinates.
	*	Follows OpenGL's coordinate rule.
	*/
	enum class Pivot
	{
		// Default. (0.0f, 0.0f, 0.0f)
		eCenter = 0,
		// (-0.5f, 0.0f)
		eLeft,
		// (0.5f, 0.0f)
		eRight,
		// (0.0f, 0.5f)
		eTop,
		// (0.0f, -0.5f)
		eBottom,
		// (-0.5f, 0.5f)
		eLeftTop,
		// (-0.5f, -0.5f)
		eLeftBottom,
		// (0.5f, 0.5f)
		eRightTop,
		// (0.5f, -0.5f)
		eRightBottom,
	};

	inline std::string to_string(v3d::Pivot value)
	{
		switch (value)
		{
		case v3d::Pivot::eCenter: return "Center";
		case v3d::Pivot::eLeft: return "Left";
		case v3d::Pivot::eRight: return "Right";
		case v3d::Pivot::eTop: return "Top";
		case v3d::Pivot::eBottom: return "Bottom";
		case v3d::Pivot::eLeftTop: return "LeftTop";
		case v3d::Pivot::eLeftBottom: return "LeftBottom";
		case v3d::Pivot::eRightTop: return "RightTop";
		case v3d::Pivot::eRightBottom: return "RightBottom";
		default: return "Invalid pivot";
		}
	}
}

#endif