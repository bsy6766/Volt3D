#include <iostream>

#include <Volt3D.h>

#include "AppDelegate.h"

int main()
{
	v3d::Application& app = AppDelegate::getInstance();
	app.init( L"Volt3D_Test", u8"Volt3D_Test" );
	app.run();
	return 0;
}