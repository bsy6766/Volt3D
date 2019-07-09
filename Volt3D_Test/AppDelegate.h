#ifndef APP_DELEGATE_H
#define APP_DELEGATE_H

#include "Volt3D/Core/Application.h"

class AppDelegate : public v3d::Application
{
private:
public:
	AppDelegate(const wchar_t* appName);
	~AppDelegate();
};

#endif