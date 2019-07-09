/**
*	@file Application.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_APPLICATION_H
#define V3D_APPLICATION_H

#include <string>
#include <string_view>

#include "Utils/Macros.h"

namespace v3d
{
	class Engine;

	class VOLT3D_DLL Application
	{
	protected:
		std::string name;

	public:
		Application() = delete;
		Application(const char* windowTitle);
		~Application();

		void start();
		void start(v3d::Engine& engine);

		std::string getName() const;
	};
}

#endif