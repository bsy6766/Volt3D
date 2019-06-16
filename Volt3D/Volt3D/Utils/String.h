/**
*	@file String.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#include <string>
#include <atlstr.h>
#include <algorithm>

#include <glm/glm.hpp>

namespace v3d
{
	namespace String
	{
		/**
		*	Finds and removes file extention from file name.
		*	It will look for last occuring '.' in the file name and removes all characters to the end in the string.
		*/
		static std::string removeFileExtFromFileName(const std::string& fileName)
		{
			const std::size_t lastIndex = fileName.find_last_of(".");
			if (lastIndex == std::string::npos)		return fileName;
			else									return fileName.substr(0, lastIndex);
		}

		static void fileNameToNameAndExt(const std::string& fileName, std::string& name, std::string& ext)
		{
			std::size_t lastindex = fileName.find_last_of(".");

			if (lastindex == std::string::npos) { name = fileName; }
			else
			{
				name = fileName.substr(0, lastindex);
				ext = fileName.substr(lastindex + 1);
			}
		}

		// trim from end (in place)
		static void rtrim(std::string &s)
		{
			s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
				return !(ch == 32);
			}).base(), s.end());
		}

		// float to string with up to 2 decimal points
		static std::string floatToStrTwoDPoints(const float value)
		{
			auto strF = std::to_string(value);
			return strF.substr(0, (strF).find_first_of('.') + 3);
		}

		static std::string toLower(std::string str)
		{
			std::transform(str.begin(), str.end(), str.begin(), ::tolower);
			return str;
		}

		// Convert std::string to std::wstring
		static std::wstring stow(const std::string &str)
		{
			if (str.empty())
				return std::wstring();

			int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
			std::wstring wstrTo(size_needed, 0);
			MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
			return wstrTo;
		}

		//	Convert std::wstring to std::string
		static std::string wtos(const std::wstring& wstr)
		{
			if (wstr.empty()) return "";
			int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
			std::string strTo(size_needed, 0);
			WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
			return strTo;
		}

		// Glm to string

		//// glm vec2	to string in (x, y) format
		//static std::string vec2ToStr(const glm::vec2& v)
		//{
		//	return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
		//}

		//// glm vec3	to string in (x, y, z) format
		//static std::string vec3ToStr(const glm::vec3& v)
		//{
		//	return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
		//}

		//// glm vec4	to string in (x, y, z, w) format
		//static std::string vec4ToStr(const glm::vec4& v)
		//{
		//	return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ", " + std::to_string(v.w) + ")";
		//}

		//// glm mat3 to string 
		//static std::string mat3ToStr(const glm::mat3& m)
		//{
		//	return "[" + std::to_string(m[0][0]) + "][" + std::to_string(m[0][1]) + "][" + std::to_string(m[0][2]) + "]\n" +
		//		"[" + std::to_string(m[1][0]) + "][" + std::to_string(m[1][1]) + "][" + std::to_string(m[1][2]) + "]\n" +
		//		"[" + std::to_string(m[2][0]) + "][" + std::to_string(m[2][1]) + "][" + std::to_string(m[2][2]) + "]";
		//}

		//// glm mat4 to string 
		//static std::string mat4ToStr(const glm::mat4& m)
		//{
		//	return "[" + std::to_string(m[0][0]) + "][" + std::to_string(m[0][1]) + "][" + std::to_string(m[0][2]) + "][" + std::to_string(m[0][3]) + "]\n" +
		//		"[" + std::to_string(m[1][0]) + "][" + std::to_string(m[1][1]) + "][" + std::to_string(m[1][2]) + "][" + std::to_string(m[1][3]) + "]\n" +
		//		"[" + std::to_string(m[2][0]) + "][" + std::to_string(m[2][1]) + "][" + std::to_string(m[2][2]) + "][" + std::to_string(m[2][3]) + "]\n" +
		//		"[" + std::to_string(m[3][0]) + "][" + std::to_string(m[3][1]) + "][" + std::to_string(m[3][2]) + "][" + std::to_string(m[3][3]) + "]";
		//}
	}
}