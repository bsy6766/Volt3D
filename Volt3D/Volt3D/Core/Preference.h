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

		bool init(std::string_view folderName);
		bool load(std::string_view folderName);

	public:
		DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Preference);
		DEFAULT_MOVE_CONSTRUCTORS(Preference);
		~Preference();

		template<class T>
		std::optional<T> get(std::string_view key) const
		{
			return std::optional<T>();
		}
	};
}

#endif