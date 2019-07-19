#ifndef APP_DELEGATE_H
#define APP_DELEGATE_H

#include "Volt3D/Core/Application.h"

class AppDelegate : public v3d::Application
{
public:
	AppDelegate();
	~AppDelegate();

	virtual bool init( const wchar_t* appName, const char* windowTitle ) override;
};

#endif