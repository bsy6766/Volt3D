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
		static bool exists(const wchar_t* path) { return std::filesystem::exists(path); }
		static bool isFile(const wchar_t* path) { return std::filesystem::is_regular_file(path); }
		static bool isDirectory(const wchar_t* path) { return std::filesystem::is_directory(path); }
		static bool isEmpty(const wchar_t* path) { return std::filesystem::is_empty(path); }
		static bool createDirectory(const wchar_t* path) { return std::filesystem::create_directory(path); }
		static bool renameFile(const wchar_t* path, const wchar_t* newFileName) { if (isFile(path)) { std::filesystem::rename(path, std::filesystem::path(getParentDir(path)).append(newFileName));  return true; } else return false; }
		static bool remove(const wchar_t* path) { return std::filesystem::remove(path); }
		static std::wstring getParentDir(const wchar_t* path) { return std::filesystem::path(path).parent_path().wstring(); }
	};

}
#endif