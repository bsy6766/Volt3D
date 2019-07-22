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

	return true;
}
