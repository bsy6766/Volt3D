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
		static bool existsW(const wchar_t* path) { return std::filesystem::exists(path); }
		static bool isFile(const wchar_t* path) { return std::filesystem::is_regular_file(path); }
		static bool isDirectory(const wchar_t* path) { return std::filesystem::is_directory(path); }
		static bool isEmpty(const wchar_t* path) { return std::filesystem::is_empty(path); }
		static bool createDirectory(const char* path) { return std::filesystem::create_directory(path); }
		static bool createDirectoryW(const wchar_t* path) { return std::filesystem::create_directory(path); }
		static bool renameFile(const wchar_t* path, const wchar_t* newFileName) { if (isFile(path)) { std::filesystem::rename(path, std::filesystem::path(getParentDirW(path)).append(newFileName));  return true; } else return false; }
		static bool remove(const wchar_t* path) { return std::filesystem::remove(path); }
		static std::string getParentDir(const char* path) { return std::filesystem::path(path).parent_path().string(); }
		static std::wstring getParentDirW(const wchar_t* path) { return std::filesystem::path(path).parent_path().wstring(); }
		static std::string getFileName(const char* path) { return std::filesystem::path(path).filename().string(); }
		static std::wstring getFileNameW(const wchar_t* path) { return std::filesystem::path(path).filename().wstring(); }
		static std::string getFileExtension(const char* path) { return std::filesystem::path(path).extension().string(); }
		static std::wstring getFileExtensionW(const wchar_t* path) { return std::filesystem::path(path).extension().wstring(); }
	};

}
#endif