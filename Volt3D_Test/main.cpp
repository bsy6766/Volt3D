#include <iostream>

#include <Volt3D.h>

#include "AppDelegate.h"

int main()
{
	AppDelegate app;
	app.init( L"Volt3D_Test", u8"Volt3D_Test" );
	app.run();
	return 0;
}