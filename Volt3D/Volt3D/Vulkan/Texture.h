///**
//*	@file Texture.h
//*
//*	@author Seung Youp Baek
//*	@copyright Copyright (c) 2019 Seung Youp Baek
//*/
//
//#ifndef V3D_VK_TEXTURE_H
//#define V3D_VK_TEXTURE_H
//
//#include <vulkan/vulkan.hpp>
//
//namespace v3d
//{
//	namespace vulkan
//	{
//		class Context;
//		class Devices;
//
//		class Texture
//		{
//			friend class Context;
//
//		private:
//			Texture();
//
//			void release();
//
//		protected:
//			static std::shared_ptr<v3d::vulkan::Devices> devices;
//
//			vk::Image image;
//			vk::ImageView imageView;
//			vk::ImageLayout imageLayout;
//			vk::DeviceMemory deviceMemory;
//			vk::Sampler sampler;
//
//			bool init( const uint32_t width, const uint32_t height, const void* data, const uint64_t dataSize );
//
//			void createImage( const uint32_t width, const uint32_t height, const vk::ImageType imageType, const vk::Format format, const vk::ImageTiling tilling, const vk::ImageUsageFlags usageFlags );
//			void allocateAndBindMemoryToImage( const vk::MemoryPropertyFlags memoryPropertyFlags );
//			void createImageView( const vk::ImageViewType imageViewType, const vk::Format format );
//			void transitionImageLayout( const vk::Format& format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout );
//
//		public:
//			~Texture();
//
//		};
//	}
//}
//
//#endif