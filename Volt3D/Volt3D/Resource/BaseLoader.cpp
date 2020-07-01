/**
*	@file BaseLoader.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "BaseLoader.h"

V3D_NS_BEGIN

BaseLoader::BaseLoader()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info( "[BaseLoader] Created" );
#endif
}

BaseLoader::~BaseLoader()
{
#ifdef BUILD_DEBUG
	v3d::Logger::getInstance().info( "[BaseLoader] Destroyed" );
#endif
}

V3D_NS_END