///**
//*	@file Texture.h
//*
//*	@author Seung Youp Baek
//*	@copyright Copyright (c) 2019 Seung Youp Baek
//*/
//
//#ifndef V3D_TEXTURE_H
//#define V3D_TEXTURE_H
//
//#include <memory>
//
//#include "Utils/Macros.h"
//
//namespace v3d
//{
//	class Image;
//	namespace vulkan { class Texture; }
//
//	/**
//	*	@class Texture
//	*	@brief A base class for all texture class in Volt3D
//	*	
//	*	@group volt3d
//	*/
//	class VOLT3D_DLL Texture
//	{
//	protected:
//		Texture();
//
//		unsigned int id;
//		static unsigned int idCounter;
//
//		const char* path;
//		unsigned int width;
//		unsigned int height;
//		unsigned int channels;
//		unsigned int mipLevel;
//		unsigned int layerCount;
//
//		v3d::vulkan::Texture* vulkanTexture;
//
//		virtual bool init( const v3d::Image& imgSrc ) = 0;
//
//	public:
//		virtual ~Texture();
//
//		const char* getPath() const { return path; }
//		unsigned int getWidth() const { return width; }
//		unsigned int getHeight() const { return height; }
//		unsigned int getChannels() const { return channels; }
//		unsigned int getMipLevel() const { return mipLevel; }
//		unsigned int getLayerCount() const { return layerCount; }
//	};
//
//	class Texture2D : public v3d::Texture
//	{
//	private:
//		Texture2D();
//
//		bool init( const v3d::Image& imgSrc ) override;
//
//	public:
//		~Texture2D();
//
//		static std::shared_ptr<v3d::Texture2D> create( const char* path );
//	};
//}
//
//#endif