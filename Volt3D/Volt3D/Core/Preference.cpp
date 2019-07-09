/**
*	@file Preference.cpp
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <PreCompiled.h>

#include "Preference.h"

#include <WinBase.h>	// GetUserName
#include <Lmcons.h>		// UNLEN
#include <ShlObj.h>		// SHGetFolderPath

#include "Utils/FileSystem.h"
#include "Utils/Logger.h"

using json = nlohmann::json;

v3d::Preference::Preference()
	: path()
{}

bool v3d::Preference::init(const std::wstring& folderName)
{
	// buffer
	wchar_t documentsPath[MAX_PATH];

	// Get ducments path
	auto result = SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, documentsPath);

	// Check result
	if (result != S_OK)
	{
		v3d::Logger::getInstance().critical("Failed to get My Documents folder path");
		return false;
	}

	path = std::wstring(documentsPath) + L"\\my games\\" + folderName;

	if (!v3d::FileSystem::exists(path) && !v3d::FileSystem::createDirectory(path)) return false;

	v3d::Logger::getInstance().info(L"Preference file path: " + path);

	return load();
}

bool v3d::Preference::reset()
{
	if (v3d::FileSystem::exists(path)) v3d::FileSystem::removeFile(path);

	std::ifstream file(path);
	if (!file.is_open()) return false;

	file.close();

	return true;
}

bool v3d::Preference::load()
{
	if (path.empty()) return false;

	const std::wstring fileName = path + L"\\preference.json";

	if (v3d::FileSystem::exists(fileName))
	{
		// open existing file
	}
	else
	{
		reset();
	}

	return true;
}
