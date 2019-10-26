/**
*	@file FileSystem.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_FILE_SYSTEM_H
#define V3D_FILE_SYSTEM_H

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>
#include <fstream>

#include "Utils/Macros.h"

namespace v3d
{
	/**
	*	@class FileSystem
	*	@brief A simple wrapper for the C++17 fileSystem.
	*/
	class VOLT3D_DLL FileSystem
	{
	public:
		FileSystem() = default;
		~FileSystem() = default;

		static std::wstring getWorkingDirectoryW() { return std::filesystem::current_path().wstring(); }

		static bool exists(const char* path) { return std::filesystem::exists(path); }
		static bool exists(std::string_view path) { return std::filesystem::exists(path); }
		static bool exists(const wchar_t* path) { return std::filesystem::exists(path); }
		static bool exists(std::wstring_view path) { return std::filesystem::exists(path); }

		static bool isFile(const char* path) { return std::filesystem::is_regular_file(path); }
		static bool isFile(std::string_view path) { return std::filesystem::is_regular_file(path); }
		static bool isFile(const wchar_t* path) { return std::filesystem::is_regular_file(path); }
		static bool isFile(std::wstring_view path) { return std::filesystem::is_regular_file(path); }

		static bool isDirectory(const wchar_t* path) { return std::filesystem::is_directory(path); }
		static bool isEmpty(const wchar_t* path) { return std::filesystem::is_empty(path); }

		static bool createDirectory(const char* path) { return std::filesystem::create_directory(path); }
		static bool createDirectory(std::string_view path) { return std::filesystem::create_directory(path); }
		static bool createDirectory(const wchar_t* path) { return std::filesystem::create_directory(path); }
		static bool createDirectory(std::wstring_view path) { return std::filesystem::create_directory(path); }

		static bool renameFile(const wchar_t* path, const wchar_t* newFileName) { if (isFile(path)) { std::filesystem::rename(path, std::filesystem::path(getParentDirW(path)).append(newFileName));  return true; } else return false; }
		
		static bool remove(const wchar_t* path) { return std::filesystem::remove(path); }

		static bool removeFile(const char* path) { if (std::filesystem::is_regular_file(path)) return std::filesystem::remove(path); else return false; }
		static bool removeFile(std::string_view path) { if (std::filesystem::is_regular_file(path)) return std::filesystem::remove(path); else return false; }
		static bool removeFile(const wchar_t* path) { if (std::filesystem::is_regular_file(path)) return std::filesystem::remove(path); else return false; }
		static bool removeFile(std::wstring_view path) { if (std::filesystem::is_regular_file(path)) return std::filesystem::remove(path); else return false; }

		static std::string getParentDir(const char* path) { return std::filesystem::path(path).parent_path().string(); }
		static std::wstring getParentDirW(const wchar_t* path) { return std::filesystem::path(path).parent_path().wstring(); }
		static std::string getFileName(const char* path) { return std::filesystem::path(path).filename().string(); }
		static std::wstring getFileNameW(const wchar_t* path) { return std::filesystem::path(path).filename().wstring(); }
		static std::string getFileExtension(const char* path) { return std::filesystem::path(path).extension().string(); }
		static std::wstring getFileExtensionW(const wchar_t* path) { return std::filesystem::path(path).extension().wstring(); }
	};

}
#endif