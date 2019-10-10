#include <PreCompiled.h>

#include "Image.h"


#include "Utils/Logger.h"
#include "Utils/FileSystem.h"
#include "Config/BuildConfig.h"

#include <libpng/png.h>

V3D_NS_BEGIN

Image::Image()
	: type(ImageType::eUndefined)
	, width(0)
	, height(0)
	, filePath()
	, data(0)
{}

Image::~Image()
{
	if (data) delete[] data;
}

Image * Image::createPNG(const std::filesystem::path& imageFilePath)
{
	Image* newImage = new (std::nothrow) Image();
	if (newImage == nullptr) return nullptr;
	else if (newImage->initPNG(imageFilePath)) return newImage;
	else return nullptr;
}

Image * Image::createPNG(const std::filesystem::path& imageFilePath, const int width, const int height, unsigned char * data)
{
	Image* newImage = new (std::nothrow) Image();
	if (newImage == nullptr) return nullptr;
	else if (newImage->initPNGWithData(imageFilePath, width, height, data)) return newImage;
	else return nullptr;
}

bool Image::initPNG(const std::filesystem::path& imageFilePath)
{
	if (imageFilePath.empty()) return false;
	
	// Read as file
	FILE* fp = fopen( imageFilePath.string().c_str(), "rb");
	if (fp == nullptr) return false;

	filePath = imageFilePath;
	
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

	width = png_get_image_width(png, info);
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
	std::size_t colSize = width * 4u; // 4 = RGBA
	data = new unsigned char[colSize * height];

	// copy
	std::size_t index = 0;
	for (std::size_t y = 0; y < height; y++)
	{
		index = y * colSize;
		memcpy(data + index, row_pointers[y], colSize);
	}
	
	fclose(fp);

	png_destroy_read_struct(&png, &info, nullptr);
	png = nullptr;
	info = nullptr;

	// Set type to png
	type = ImageType::ePNG;

	// Done.
	return true;
}

bool Image::initPNGWithData(const std::filesystem::path & imageFilePath, const int width, const int height, unsigned char * data)
{
	// check path
	if (imageFilePath.empty()) return false;
	if (data == 0) return false;
	if (width == 0 || height == 0) return false;
	
	this->filePath = imageFilePath;
	this->width = width;
	this->height = height;

	const std::size_t size = width * height * 4u;
	this->data = new uint8_t[size];
	memcpy(this->data, data, size);

	type = ImageType::ePNG;

	return true;
}

bool Image::write()
{
	return write(filePath);
}

bool Image::write(const std::filesystem::path& imageFilePath)
{
	// Check path
	if (imageFilePath.empty()) return false;
	
	std::filesystem::path dst = imageFilePath;
	std::filesystem::path parentDir = dst.parent_path();

	if (parentDir.empty()) parentDir = dst;
	if (!std::filesystem::exists( parentDir )) std::filesystem::create_directory( parentDir );

	// @todo: Check if filpath already contains png extension.
	if (!dst.has_extension()) dst.append( ".png" );

	// read file
	FILE *fp = fopen( dst.string().c_str(), "wb");
	if (fp == nullptr) return false;

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
	png_uint_32 w = 0;
	png_uint_32 h = 0;
	png_set_IHDR(png, info, w, h, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, info);

	width = std::size_t( w );
	height = std::size_t( h );

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
	
	png_write_image(png, row_pointers);
	free(row_pointers);
	row_pointers = nullptr;
	png_write_end(png, info);
	png_destroy_write_struct(&png, &info);

	fclose(fp);

	return true;
}

void Image::flipImage()
{
	unsigned long rowSize = width * 4u; //RGBA
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

vk::Format Image::getVKFormat() const
{
	switch (type)
	{
	case v3d::ImageType::ePNG:					return vk::Format::eR8G8B8A8Unorm;
	// @todo Add more type
	case v3d::ImageType::eUndefined: default:	return vk::Format::eR8G8B8A8Unorm;
	}
}

void Image::print() const
{
	auto& logger = v3d::Logger::getInstance();

	logger.info("[Image] Info");
	logger.info("File: " + filePath.string());
	logger.info("Type: " + imageTypeToString(type));
	logger.info("Width: " + std::to_string(width));
	logger.info("Height: " + std::to_string(height));
}

V3D_NS_END