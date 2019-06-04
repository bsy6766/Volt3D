/**
*	@file Time.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_GLFW_TIME_H
#define V3D_GLFW_TIME_H

#include <vulkan/vulkan.hpp>

#include <functional>

#include "utils/Macros.h"

namespace v3d
{
	namespace glfw
	{
		/**
		*	@class Time
		*	@brief Wrapper for GLFW time functions.
		*
		*	@group glfw
		*
		*	@since 1.0
		*/
		class VOLT3D_DLL Time
		{
			friend class Engine;

		private:
			Time();

			double currentTime;
			double previousTime;
			double elapsedTime;
			int fps;
			double fpsElapsedTime;

			void updateTime();
			void updateFPS();

			std::function<void(const int)> onFPSUpdated;

		public:
			DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Time);
			DEFAULT_MOVE_CONSTRUCTORS(Time);
			~Time(){}

			void resetTime();
			int getFPS() const;
			double getElaspedTime() const;
			double getCurrentTime() const;
			void setOnFPSUpdateCallback(const std::function<void(const int)>* func);
		};
	}
}

#endif