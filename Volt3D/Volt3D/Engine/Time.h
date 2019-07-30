/**
*	@file Time.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_TIME_H
#define V3D_TIME_H

#include <functional>

#include "utils/Macros.h"

V3D_NS_BEGIN

class Engine;

/**
*	@class Time
*	@brief Wrapper for GLFW time functions.
*
*	@group Engine
*
*	@since 1.0
*/
class VOLT3D_DLL Time
{
	friend class v3d::Engine;

private:
	Time();

	double currentTime;
	double previousTime;
	double elapsedTime;
	int fps;
	double fpsElapsedTime;

	void updateTime();
	void updateFPS();

	std::function<void( const int )> onFPSUpdated;

public:
	~Time() {}
		
	DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Time );
	DEFAULT_MOVE_CONSTRUCTORS( Time );

	/** Get Time instance. */
	static v3d::Time* get();

	/** Reset time and fps to 0 */
	void reset();

	/** Get latest FPS */
	int getFPS() const;

	/** Get elapsed time on current frame */
	double getElaspedTime() const;

	/** Get current time */
	double getCurrentTime() const;

	/** Set callback function on fps update */
	void setOnFPSUpdateCallback( const std::function<void( const int )>* func );
};

V3D_NS_END

#endif