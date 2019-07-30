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
	private:


	protected:
		Application();

		std::wstring name;
		v3d::Engine* engine;

	public:
		virtual ~Application();

		DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Application );
		DEFAULT_MOVE_CONSTRUCTORS( Application );

		static v3d::Application& get()
		{
			static v3d::Application instance;
			return instance;
		}

		virtual bool init( const wchar_t* appName, const char* windowTitle );
		virtual void run();

		std::wstring getName() const { return name; }
		v3d::Engine* getEngine() const { return engine; }
	};
}

#endif