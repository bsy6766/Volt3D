#include <PreCompiled.h>

#include "Image.h"


#include "Utils/Logger.h"
#include "Utils/FileSystem.h"
#include "Config/BuildConfig.h"

#include <libpng\png.h>

v3d::Image::Image()
	: type(v3d::Image::Type::PNG)
	, width(0)
	, height(0)
	, filePath("")
	, fileName("")
	, data(0)
{}

v3d::Image::~Image()
{
	if (data)
	{
		delete[] data;
	}
}

v3d::Image * v3d::Image::createPNG(const std::string & filePath)
{
	// Create new image instance
	v3d::Image* newImage = new (std::nothrow) v3d::Image();

	// Check
	if (newImage)
	{
		// Init as PNG
		if (newImage->initPNG(filePath))
		{
			// Done.
			return newImage;
		}
		else
		{
			// Delete instance
			delete newImage;
		}
	}
	else
	{
		// report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::IMAGE);
	}

	// Failed
	return nullptr;
}

v3d::Image * v3d::Image::createPNG(const std::string & filePath, const int width, const int height, unsigned char * data)
{
	// Create new image instance
	v3d::Image* newImage = new (std::nothrow) v3d::Image();

	// Check
	if (newImage)
	{
		// Init as PNG with data
		if (newImage->initPNGWithData(filePath, width, height, data))
		{
			// Done.
			return newImage;
		}
		else
		{
			// Delete instance
			delete newImage;
		}
	}
	else
	{
		// report
		v3d::Error::Report::reportStdBadAlloc(v3d::Error::Code::Source::IMAGE);
	}

	// Failed
	return nullptr;
}

bool v3d::Image::initPNG(const std::string & filePath)
{
	// check path
	if (filePath.empty())
	{
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
		// report
		v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::IMAGE, v3d::Error::Code::IMAGE_FILE_PATH_IS_EMPTY));
#endif

		// Failed
		return false;
	}

	// get file system
	v3d::FileSystem& fs = v3d::Engine::getInstance().getFileSystem();

	// Read as file
	std::FILE *fp = fs.openFile(filePath);

	// Check
	if (fp == nullptr)
	{
#if (V3D_TARGET_BUILD == V3D_DEBUG_BUILD)
		// report
		v3d::Error::Report::report(v3d::Error::Code::generate(false, v3d::Error::Code::Source::IMAGE, v3d::Error::Code::FAILED_TO_OPEN_IMAGE_FILE));
#endif

		// Failed
		return false;
	}

	// save file name and path
	this->fileName = v3d::FileSystem::getFileNameFromPath(filePath);
	this->filePath = filePath;
	
	// Init libpng
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png)
	{
		fclose(fp);

		return false;
	}

	png_infop info = png_create_info_struct(png);
	if (!info)
	{
		png_destroy_read_struct(&png, nullptr, nullptr);
		png = nullptr;

		fclose(fp);

		return false;
	}

	if (setjmp(png_jmpbuf(png)))
	{
		png_destroy_read_struct(&png, &info, nullptr);
		png = nullptr;
		info = nullptr;

		fclose(fp);

		return false;
	}

	png_init_io(png, fp);

	png_read_info(png, info);

	// get width
	width = png_get_image_width(png, info);
	// get height
	height = png_get_image_height(png, info);

	auto color_type = png_get_color_type(png, info);
	auto bit_depth = png_get_bit_depth(png, info);

	// Read any color_type into 8bit depth, RGBA format.
	// See http://www.libpng.org/pub/png/libpng-manual.txt

	if (bit_depth == 16)
		png_set_strip_16(png);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if (png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if (color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	// create temporary data in c style because libpng is written c
	auto row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);

	for (unsigned int y = 0; y < height; y++) 
	{
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
	}

	// read png image
	png_read_image(png, row_pointers);

	// initialize data
	std::size_t colSize = width * 4; // 4 = RGBA
	data = new unsigned char[colSize * height];

	// copy
	std::size_t index = 0;
	for (unsigned int y = 0; y < height; y++)
	{
		index = y * colSize;
		memcpy(data + index, row_pointers[y], colSize);
	}
	
	fclose(fp);

	png_destroy_read_struct(&png, &info, nullptr);
	png = nullptr;
	info = nullptr;

	// Set type to png
	type = v3d::Image::Type::PNG;

	// Done.
	return true;
}

bool v3d::Image::initPNGWithData(const std::string & filePath, const int width, const int height, unsigned char * data)
{
	// check path
	if (filePath.empty())
	{
#if V3D_LOG_WARNING
		v3d::Logger::getInstance().warn("[Image] File path is empty.");
#endif
		return false;
	}

	if (data == 0)
	{
#if V3D_LOG_WARNING
		v3d::Logger::getInstance().warn("[Image] Data is empty.");
#endif
		return false;
	}

	if (width == 0 || height == 0)
	{
#if V3D_LOG_WARNING
		v3d::Logger::getInstance().warn("[Image] Size is 0.");
#endif
		return false;
	}

	this->fileName = v3d::FileSystem::getFileNameFromPath(filePath);
	this->filePath = filePath;
	this->width = width;
	this->height = height;

	std::size_t size = width * height * 4;
	this->data = new uint8_t[size];
	memcpy(this->data, data, size);

	type = v3d::Image::Type::PNG;

	return true;
}

bool v3d::Image::write()
{
	return write(filePath);
}

bool v3d::Image::write(const std::string & filePath)
{
	// Check path
	if (filePath.empty()) return false;
	
	// Check if path is regular file

	// Get parent directory. In this case, folder
	std::string folder = v3d::FileSystem::getParentDirectory(filePath);

	if (folder.empty())
	{
		folder = filePath;
	}

	if (!v3d::FileSystem::exists(folder))
	{
		// path doesn't exsits
		v3d::FileSystem::createFolder(folder);
	}

	// @todo: Check if filpath already contains png extension.

	// read file
	FILE *fp = fopen((filePath + ".png").c_str(), "wb");

	if (fp == nullptr)
	{
#if V3D_LOG_WARNING
		v3d::Logger::getInstance().warn("[Image] Can't write file");
#endif
		return false;
	}

	// init libpng

	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png)
	{
		fclose(fp);

		return false;
	}

	png_infop info = png_create_info_struct(png);

	if (!info)
	{
		png_destroy_write_struct(&png, nullptr);
		png = nullptr;

		fclose(fp);

		return false;
	}

	if (setjmp(png_jmpbuf(png)))
	{
		png_destroy_write_struct(&png, &info);
		png = nullptr;

		fclose(fp);

		return false;
	}

	png_init_io(png, fp);

	// Output is 8bit depth, RGBA format.
	png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, info);

	// To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
	// Use png_set_filler().
	//png_set_filler(png, 0, PNG_FILLER_AFTER);

	// create row_pointers
	png_bytep *row_pointers = (png_bytep *)malloc(height * sizeof(png_bytep));

	// convert data to row_pointers
	for (unsigned int i = 0; i < height; i++)
	{
		row_pointers[i] = (png_bytep)data + (i * width * 4);
	}
	
	// writ png
	png_write_image(png, row_pointers);

	// free
	free(row_pointers);
	row_pointers = nullptr;

	// end png
	png_write_end(png, info);

	// release
	png_destroy_write_struct(&png, &info);

	// close file
	fclose(fp);

	return true;
}

void v3d::Image::flipImage()
{
	unsigned long rowSize = width * 4; //RGBA
	unsigned char* rowBuffer = new unsigned char[rowSize];
	unsigned halfRows = height / 2;

	for (unsigned rowIdx = 0; rowIdx < halfRows; ++rowIdx) {
		unsigned char* row = data + ((rowIdx * width + 0) * 4);
		unsigned char* oppositeRow = data + (((height - rowIdx - 1) * width + 0) * 4);

		std::memcpy(rowBuffer, row, rowSize);
		std::memcpy(row, oppositeRow, rowSize);
		std::memcpy(oppositeRow, rowBuffer, rowSize);
	}

	delete[] rowBuffer;
}

std::string v3d::Image::getFilePath() const
{
	return filePath;
}

unsigned int v3d::Image::getWidth() const
{
	return width;
}

unsigned int v3d::Image::getHeight() const
{
	return height;
}

unsigned char * v3d::Image::getData() const
{
	return data;
}

void v3d::Image::print() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.info("[Image] Info");
	logger.info("File: " + filePath);
	logger.info("Type: " + imageTypeToString(type));
	logger.info("Width: " + std::to_string(width));
	logger.info("Height: " + std::to_string(height));
}