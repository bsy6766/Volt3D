#include "HelloVulkan.h"

HelloVulkan::HelloVulkan()
	: v3d::Scene("HelloVulkan")
{}

HelloVulkan::~HelloVulkan()
{}

bool HelloVulkan::onInit()
{
	v3d::Logger::getInstance().trace( "HelloVulkan onInit()" );

	//v3d::Image* RGBW = v3d::Image::createPNG( "Textures/RGBW.png" );
	//auto data = RGBW->getData();

	//v3d::Logger::getInstance().trace( "Image path: " + RGBW->getFilePath() );
	//v3d::Logger::getInstance().trace( "w: " + std::to_string( RGBW->getWidth() ) + ", h: " + std::to_string( RGBW->getHeight() ) );

	//for (int i = 0; i < 4; i++)
	//{
	//	v3d::Logger::getInstance().trace( "Pixel: " + std::to_string( data[i * 4] ) + ", " + std::to_string( data[i * 4 + 1] ) + ", " + std::to_string( data[i * 4 + 2] ) + ", " + std::to_string( data[i * 4 + 3] ));
	//}

	//delete RGBW;

#if V3D_DEBUG_CAMERA
	auto& app = v3d::Application::get();
	auto res = v3d::glfw::Window::get()->getWindowSize();
	auto debugCamera = std::shared_ptr<v3d::DebugCamera>( v3d::DebugCamera::create( "DebugCam", v3d::ProjectionType::ePerspective, 70.0f, 0.1f, 1000.0f, float( res.x ), float( res.y ) ) );
	app.getEngine()->getDirector()->attachDebugCamera( debugCamera );
	app.getEngine()->getDirector()->enableDebugCamera();
#endif
	return true;
}
