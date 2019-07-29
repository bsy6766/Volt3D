/**
*	@file Texture.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_VK_TEXTURE_H
#define V3D_VK_TEXTURE_H

#include <vulkan/vulkan.hpp>

#include <memory>

namespace v3d
{
	class Image;
	
	namespace vulkan
	{
		class Devices;

		class Texture
		{
		private:
			void release();

		protected:
			Texture() = delete;
			Texture(vk::Device& logicalDevice);

			vk::Device& logicalDevice;

			vk::Image image;
			vk::ImageView imageView;
			vk::ImageLayout imageLayout;
			vk::DeviceMemory deviceMemory;
			vk::Sampler sampler;

			char* path;
			uint32_t width;
			uint32_t height;
			uint32_t channels;
			uint32_t mipLevel;
			uint32_t layerCount;

			virtual bool init(const char * filePath, v3d::vulkan::Devices* devices ) = 0;
			virtual void createImage( v3d::vulkan::Devices* devices, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags, const vk::MemoryPropertyFlags memoryPropertyFlags ) = 0;

		public:
			virtual ~Texture();

			const char* getPath() const { return path; }
			uint32_t getWidth() const { return width; }
			uint32_t getHeight() const { return height; }
			uint32_t getChannels() const { return channels; }
			uint32_t getMipLevel() const { return mipLevel; }
			uint32_t getLayerCount() const { return layerCount; }
		};

		class Texture2D : public Texture
		{
		private:
			Texture2D( vk::Device& device );
			
			virtual bool init( const char* filePath, v3d::vulkan::Devices* devices ) override;
			virtual void createImage( v3d::vulkan::Devices* devices, const vk::Format& format, const vk::ImageTiling& tilling, const vk::ImageUsageFlags usageFlags, const vk::MemoryPropertyFlags memoryPropertyFlags ) override;

		public:
			static std::shared_ptr<v3d::vulkan::Texture2D> create( const char* filePath, v3d::vulkan::Devices* devices );
		};
	}
}

#endif