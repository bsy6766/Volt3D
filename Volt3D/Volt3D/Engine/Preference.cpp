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
#include "Engine/WindowMode.h"

using json = nlohmann::json;

v3d::Preference::Preference()
	: path()
	, data()
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

	path += L"\\preference.json";

	v3d::Logger::getInstance().info(L"Preference file path: " + path);

	return load();
}

bool v3d::Preference::readJson()
{
	std::ifstream file(path);
	if (!file.is_open()) return false;

	try { file >> data; }
	catch (...) { return false; }

	file.close();

	return true;
}

v3d::Preference::~Preference() {}

int v3d::Preference::getInt(const char* key)
{
	if (data.empty()) return 0;
	try 
	{
		const auto& value = data[key];
		return value;
	}
	catch (...)
	{
		return 0;
	}
}

bool v3d::Preference::reset()
{
	if (v3d::FileSystem::exists(path)) v3d::FileSystem::removeFile(path);
	data.clear();

	data["display_monitor_index"] = 0;			// primary monitor
	data["display_resolution_width"] = 1280;	// HD
	data["display_resolution_height"] = 720;	// HD
	data["display_vsync"] = 0;					// Vsync disabled
	data["display_window_mode"] = static_cast<unsigned int>(v3d::WindowMode::eWindowed);

	return true;
}

bool v3d::Preference::load()
{
	if (path.empty()) return false;
	
	if (v3d::FileSystem::exists(path))
	{
		return readJson();
	}
	else
	{
		return reset() && save();
	}
}

bool v3d::Preference::save()
{
	if (path.empty()) return false;

	if (v3d::FileSystem::exists(path)) v3d::FileSystem::removeFile(path);

	std::ofstream file(path);
	if (!file.is_open()) return false;

	file << std::setw(4) << data << std::endl;
	file.close();

	return true;
}
