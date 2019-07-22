#ifndef HELLO_VULKAN_H
#define HELLO_VULKAN_H

#include <Volt3D.h>

class HelloVulkan : public v3d::Scene
{
private:
public:
	HelloVulkan();
	~HelloVulkan();

	CREATE_SCENE( HelloVulkan );

	virtual bool onInit() override;
	virtual void onRelease() override { v3d::Logger::getInstance().trace( "onRelease" ); }
	virtual void onEnter() override { v3d::Logger::getInstance().trace( "onEnter" ); }
	virtual void onEnterFinished() override { v3d::Logger::getInstance().trace( "onEnterFinished" ); }
	virtual void onExit() override { v3d::Logger::getInstance().trace( "onExit" ); }
	virtual void onExitFinished() override { v3d::Logger::getInstance().trace( "onExitFinished" ); }
};

#endif