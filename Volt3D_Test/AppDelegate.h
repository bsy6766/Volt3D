#ifndef APP_DELEGATE_H
#define APP_DELEGATE_H

#include "Volt3D/Core/Application.h"

class AppDelegate : public v3d::Application
{
private:
	AppDelegate();
	~AppDelegate();

public:
	static v3d::Application& getInstance()
	{
		return v3d::Application::get();
	}

	virtual bool init( const wchar_t* appName, const char* windowTitle ) override;
};

#endif