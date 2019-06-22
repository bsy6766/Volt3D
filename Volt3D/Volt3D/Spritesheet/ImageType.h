/**
*	@file ImageType.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_IMAGE_TYPE_H
#define V3D_IMAGE_TYPE_H

#include <string>

namespace v3d
{
	/**
	*	@enum InputStae
	*	State of input for keyboard and mouse.
	*/
	enum class ImageType : unsigned int
	{
		ePNG,
	};

	inline std::string imageTypeToString(v3d::ImageType value)
	{
		switch (value)
		{
		case v3d::ImageType::ePNG: return "PNG";
		default: return "Invalid image type";
		}
	}
}

#endif
