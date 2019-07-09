/**
*	@file Preference.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_PREFERENCE_H
#define V3D_PREFERENCE_H

#include <string>
#include <optional>

#include <nlohmann/json.hpp>

#include "Utils/Macros.h"

namespace v3d
{
	/**
	*	@class Prefence
	*	@brief Read and write preference file 
	*
	*	@group core
	*
	*	@since 1.0
	*/
	class VOLT3D_DLL Preference
	{
		friend class Engine;

	private:
		Preference();

		std::wstring path;
		nlohmann::json data;

		bool init(const std::wstring& folderName);
		bool readJson();

	public:
		DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Preference);
		DEFAULT_MOVE_CONSTRUCTORS(Preference);
		~Preference();

		int getInt(const char* key);

		bool reset();
		bool load();
		bool save();

		std::wstring getPath() const { return path; }
		const nlohmann::json& getData() const { return data; }
	};
}

#endif