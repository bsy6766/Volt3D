/**
*	@file Image.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_IMAGE_H
#define V3D_IMAGE_H

#include <filesystem>

#include "Utils/Macros.h"
#include "ImageType.h"

V3D_NS_BEGIN

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

	std::filesystem::path filePath;

	unsigned char* data;

	bool initPNG(const std::filesystem::path& filePath);
	bool initPNGWithData(const std::filesystem::path& filePath, const int width, const int height, unsigned char* data);

public:
	// Destructor
	~Image();

	/**
	*	Read PNG image and create Image instance.
	*	@param imageFilePath A path of image file to read.
	*/
	static Image* createPNG(const std::filesystem::path& imageFilePath);

	/**
	*	Create Image with existing data
	*	@param imageFilePath A path of image file to read.
	*/
	static Image* createPNG(const std::filesystem::path& imageFilePath, const int width, const int height, unsigned char* data);

	/**
	*	Write Image to file.
	*	Supported images formats: PNG
	*/
	bool write();

	/**
	*	Write Image to file to file path
	*	Supported images formats: PNG
	*/
	bool write(const std::filesystem::path& filePath);

	/** flip image vertically. */
	void flipImage();

	/** get image file path. */
	std::filesystem::path getFilePath() const { return filePath; }

	/** get image file name */
	std::string getFileName() const { return filePath.filename().string(); }

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

V3D_NS_END

#endif