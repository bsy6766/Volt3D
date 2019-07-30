///**
//*	@file Texture.cpp
//*
//*	@author Seung Youp Baek
//*	@copyright Copyright (c) 2019 Seung Youp Baek
//*/
//
#include <PreCompiled.h>
//
//#include "Texture.h"
//
//#include "Vulkan/Texture.h"
//#include "Spritesheet/Image.h"
//
//unsigned int v3d::Texture::idCounter = 0;
//
//v3d::Texture::Texture()
//	: id(v3d::Texture::idCounter++)
//	, path(nullptr)
//	, width(0)
//	, height(0)
//	, channels(0)
//	, mipLevel(0)
//	, layerCount(0)
//	, vulkanTexture(nullptr)
//{}
//
//v3d::Texture::~Texture()
//{}
//
//
//
//v3d::Texture2D::Texture2D()
//	: v3d::Texture()
//{}
//
//v3d::Texture2D::~Texture2D()
//{}
//
//bool v3d::Texture2D::init( const v3d::Image& imgSrc )
//{
//	//vulkanTexture = v3d::vulkan::Texture2D()
//
//	path = imgSrc.getFilePath().c_str();
//	width = imgSrc.getWidth();
//	height = imgSrc.getHeight();
//	channels = imgSrc.getHeight();
//	mipLevel = 1;
//	layerCount = 1;
//
//
//
//	return false;
//}
//
//std::shared_ptr<v3d::Texture2D> v3d::Texture2D::create( const char* path )
//{
//	if (path == nullptr) return nullptr;
//
//	auto imgSrc = std::unique_ptr<v3d::Image>( v3d::Image::createPNG( path ) );
//	if (imgSrc == nullptr) return nullptr;
//
//	auto newTexture = new (std::nothrow) v3d::Texture2D();
//	if (newTexture == nullptr) return nullptr;
//
//	if (newTexture->init( *imgSrc.get() )) return std::shared_ptr<v3d::Texture2D>( newTexture );
//	else delete newTexture;
//
//	return nullptr;
//}