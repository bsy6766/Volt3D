/**
*	@file Error.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2020 Seung Youp Baek
*/

#ifndef V3D_ERROR_CODES_H
#define V3D_ERROR_CODES_H

#include <system_error>
#include <iostream>

namespace v3d
{
	// ==== Engine Error ================================================
	enum class EngineErr
	{
		eNoError = 0,
	};

	struct EngineErrCat : std::error_category
	{
		const char* name() const noexcept override { return "Shader"; }
		std::string message(int ev) const override
		{
			switch (static_cast<v3d::EngineErr>(ev))
			{
			case v3d::EngineErr::eNoError:			return "no error.";
			default:								return "unknown error.";
			}
		}
	};
	// ==================================================================

	// ==== Shader Error ================================================
	enum class ShaderErr
	{
		eNoError = 0,
		eFileNotFound = 1,
	};

	struct ShaderErrCat : std::error_category
	{
		const char* name() const noexcept override { return "Shader"; }
		std::string message(int ev) const override
		{
			switch (static_cast<v3d::ShaderErr>(ev))
			{
			case v3d::ShaderErr::eNoError:			return "no error.";
			case v3d::ShaderErr::eFileNotFound:		return "file not found.";
			default:								return "unknown error.";
			}
		}
	};
	// ==================================================================

	// ==== Texture Error ===============================================
	enum class TextureErr
	{
		eNoError = 0,
		eFileNotFound = 1,
		eUnsupportedFormat = 2,
	};

	struct TextureErrCat : std::error_category
	{
		const char* name() const noexcept override { return "Texture"; }
		std::string message(int ev) const override
		{
			switch (static_cast<v3d::TextureErr>(ev))
			{
			case v3d::TextureErr::eNoError:				return "no error.";
			case v3d::TextureErr::eFileNotFound:		return "file not found.";
			case v3d::TextureErr::eUnsupportedFormat:	return "unsupported format.";
			default:									return "unknown error.";
			}
		}
	};
	// ==================================================================

	const v3d::EngineErrCat engineErrCategory{};
	const v3d::ShaderErrCat shaderErrCategory{};
	const v3d::TextureErrCat textureErrCategory{};

	std::error_code make_error_code(v3d::EngineErr);
	std::error_code make_error_code(v3d::ShaderErr);
	std::error_code make_error_code(v3d::TextureErr);

	class Error
	{
	private:
		std::error_code code;
		std::string fileName;
		int fileLine;
	public:
		Error() = delete;
		Error(const std::error_code& code, const std::string& fileName, const int fileLine) : code(code), fileName(fileName), fileLine(fileLine) {}
		~Error() = default;

		void log()
		{
			std::cout << "-- Error code --\n";
			std::cout << "- Category: " << code.category().name() << "\n";
			std::cout << "- Message: " << code.message() << "\n";
			std::cout << "- Value: " << code.value() << "\n";
			if (!fileName.empty()) std::cout << "- File name: " << fileName << "\n";
			if (fileLine) std::cout << "- File line: " << fileLine << "\n";
			std::cout << "----------------\n";
		}
	};

	static v3d::Error generateError(const std::error_code& code)
	{ return v3d::Error(code, "", -1); }
	static v3d::Error generateError(const std::error_code& code, const std::string& fileName)
	{ return v3d::Error(code, fileName, -1); }
	static v3d::Error generateError(const std::error_code& code, const std::string& fileName, const int fileLine)
	{ return v3d::Error(code, fileName, fileLine); }
}

namespace std
{
	template <> struct is_error_code_enum<v3d::EngineErr> : true_type {};
	template <> struct is_error_code_enum<v3d::ShaderErr> : true_type {};
	template <> struct is_error_code_enum<v3d::TextureErr> : true_type {};
}


#endif