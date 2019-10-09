/**
*	@file Image.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_IMAGE_H
#define V3D_IMAGE_H

#include "Utils/Macros.h"
#include "ImageType.h"

namespace v3d
{
	/**
	*	@class Image
	*	Simple class that contains image information.
	* 
	*	At this point, only supports PNG.
	* 
	*	External libraries
	*	PNG: libpng
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Image
	{
	private:
		// Constructor
		Image();

		// Type of image
		v3d::ImageType type;

		unsigned int width;
		unsigned int height;

		std::string filePath;
		std::string fileName;

		unsigned char* data;

		bool initPNG(const std::string& filePath);
		bool initPNGWithData(const std::string& filePath, const int width, const int height, unsigned char* data);

	public:
		// Destructor
		~Image();

		/**
		*	Read PNG image and create Image instance.
		*	@param filePath A path of image file to read.
		*/
		static Image* createPNG(const std::string& filePath);

		/**
		*	Create Image with existing data
		*	@param filePath A path of image file to read.
		*/
		static Image* createPNG(const std::string& filePath, const int width, const int height, unsigned char* data);

		/**
		*	Write Image to file.
		*	Supported images formats: PNG
		*/
		bool write();

		/**
		*	Write Image to file to file path
		*	Supported images formats: PNG
		*/
		bool write(const std::string& filePath);

		/** flip image vertically. */
		void flipImage();

		/** get image file path. */
		std::string getFilePath() const { return filePath; }

		/** get image file name */
		std::string getFileName() const { return fileName; }

		/** get width. */
		unsigned int getWidth() const { return width; }

		/** get height. */
		unsigned int getHeight() const { return height; }

		/** get channel (RGBA for png) */
		// @todo: Return correct channels for each image type
		unsigned int getChannels() const { return 4; }

		/** get image data size **/
		unsigned int getDataSize() const { return width* height * getChannels(); }

		/** get data. */
		unsigned char* getData() const { return data; }

		/** Get Vulkan Format. */
		vk::Format getVKFormat() const;

		/** Log iamge. For debug. */
		void print() const;
	};
}

#endif