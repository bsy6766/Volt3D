/**
*	@file Texture.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_TEXTURE_H
#define V3D_TEXTURE_H

namespace v3d
{
	class Texture
	{
	protected:
		Texture();

		char* path;
		unsigned int width;
		unsigned int height;
		unsigned int channels;
		unsigned int mipLevel;
		unsigned int layerCount;

	public:
		virtual ~Texture();

		const char* getPath() const { return path; }
		unsigned int getWidth() const { return width; }
		unsigned int getHeight() const { return height; }
		unsigned int getChannels() const { return channels; }
		unsigned int getMipLevel() const { return mipLevel; }
		unsigned int getLayerCount() const { return layerCount; }
	};

	class Texture2D : public v3d::Texture
	{
	private:
		Texture2D();
	public:
		~Texture2D();
	};
}

#endif