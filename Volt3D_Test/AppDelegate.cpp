#include "AppDelegate.h"

#include <volt3D.h>
#include "HelloVulkan.h"

AppDelegate::AppDelegate()
	: v3d::Application()
{}

AppDelegate::~AppDelegate() {}

bool AppDelegate::init( const wchar_t* appName, const char* windowTitle )
{
	if (!v3d::Application::init( appName, windowTitle )) return false;
	auto dir = engine->getDirector();
	dir->runScene( std::move(std::shared_ptr<HelloVulkan>(HelloVulkan::create())) );
	return true;
}
