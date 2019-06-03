/**
*	@file Logger.h
*
*	@author Seung Youp Baek
*	@copyright Copyright (c) 2019 Seung Youp Baek
*/

#ifndef V3D_LOGGER_H
#define V3D_LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <vulkan/vulkan.hpp>

#include <memory>
#include <filesystem>
#include <fstream>

#include "Macros.h"
#include "Config/BuildConfig.h"

namespace spdlog
{
	class logger;
}

namespace v3d
{
	class Logger
	{
	private:
		std::shared_ptr<spdlog::logger> logger;
		std::shared_ptr<spdlog::logger> console;

		Logger() : logger(nullptr), console(nullptr) {}
		~Logger() { logger->flush(); if (console) console->flush(); }

	public:
		DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR(Logger);
		DEFAULT_MOVE_CONSTRUCTORS(Logger);

		static Logger& getInstance()
		{
			static Logger instance;
			return instance;
		}

		bool init(const std::wstring& filePath, const std::wstring& fileName)
		{
			if (logger) return false;

			std::filesystem::path loggerPath(filePath.empty() ? std::filesystem::current_path().c_str() : filePath);
			loggerPath /= std::filesystem::path(fileName.empty() ? L"log" : fileName);

			if (std::filesystem::exists(loggerPath)) std::filesystem::remove(loggerPath);

			std::wifstream ifs(loggerPath);
			ifs.close();

			logger = spdlog::basic_logger_mt("Log", loggerPath.c_str());
			logger->set_pattern("[%l] [%c] %v");
			logger->set_level(spdlog::level::trace);
						
#ifdef BUILD_DEBUG
			logger->trace("Trace test");
			logger->debug("Debug test");
			logger->info("Info test");
			logger->warn("Warning test");
			logger->error("Error test");
			logger->critical("Critical test");
#endif

			logger->trace((std::wstring(L"Created log file at: ") + loggerPath.wstring()).c_str());
			logger->flush();

			return true;
		}

		bool initConsole()
		{
			if (console) return false;
			console = spdlog::stdout_color_mt("Console");
			console->set_pattern("%^[%l] [%c] %v%$");
			console->set_level(spdlog::level::trace);

#ifdef BUILD_DEBUG
			console->trace("Trace test");
			console->debug("Debug test");
			console->info("Info test");
			console->warn("Warning test");
			console->error("Error test");
			console->critical("Critical test");
#endif

			return true;
		}

		void setPattern(const std::string & pattern) const { if (logger) logger->set_pattern(pattern.c_str()); }
		void setConsolePattern(const std::string & pattern) const { if (console) console->set_pattern(pattern.c_str()); }

		void trace(const std::string& msg) const { if (logger) logger->trace(msg); traceConsole(msg); }
		void trace(const std::wstring& msg) const { if (logger) logger->trace(msg.c_str()); traceConsole(msg.c_str()); }
		void traceConsole(const std::string& msg) const { if (console) console->trace(msg); }
		void traceConsole(const std::wstring& msg) const { if (console) console->trace(msg.c_str()); }

		void debug(const std::string& msg) const { if (logger) logger->debug(msg); debugConsole(msg); }
		void debug(const std::wstring& msg) const { if (logger) logger->debug(msg.c_str()); debugConsole(msg.c_str()); }
		void debugConsole(const std::string& msg) const { if (console) console->debug(msg); }
		void debugConsole(const std::wstring& msg) const { if (console) console->debug(msg.c_str()); }

		void info(const std::string& msg) const { if (logger) logger->info(msg); infoConsole(msg); }
		void info(const std::wstring& msg) const { if (logger) logger->info(msg.c_str()); infoConsole(msg.c_str()); }
		void infoConsole(const std::string& msg) const { if (console) console->info(msg); }
		void infoConsole(const std::wstring& msg) const { if (console) console->info(msg.c_str()); }

		void warn(const std::string& msg) const { if (logger) logger->warn(msg); warnConsole(msg); }
		void warn(const std::wstring& msg) const { if (logger) logger->warn(msg.c_str()); warnConsole(msg.c_str()); }
		void warnConsole(const std::string& msg) const { if (console) console->warn(msg); }
		void warnConsole(const std::wstring& msg) const { if (console) console->warn(msg.c_str()); }

		void error(const std::string& msg) const { if (logger) logger->error(msg); errorConsole(msg); }
		void error(const std::wstring& msg) const { if (logger) logger->error(msg.c_str()); errorConsole(msg.c_str()); }
		void errorConsole(const std::string& msg) const { if (console) console->error(msg); }
		void errorConsole(const std::wstring& msg) const { if (console) console->error(msg.c_str()); }

		void critical(const std::string& msg) const { if (logger) logger->critical(msg); criticalConsole(msg); }
		void critical(const std::wstring& msg) const { if (logger) logger->critical(msg.c_str()); criticalConsole(msg.c_str()); }
		void criticalConsole(const std::string& msg) const { if (console) console->critical(msg); }
		void criticalConsole(const std::wstring& msg) const { if (console) console->critical(msg.c_str()); }

		template<class T>
		void bad_alloc() { if (logger) logger->critical(std::string("std::bad_alloc on ") + std::string(typeid(T).name())); }

#ifdef BUILD_DEBUG
		void logExtensions(const std::vector<vk::ExtensionProperties> & extensions) const
		{
			logger->trace("Available VK extensions: " + std::to_string(extensions.size()));
			for (const auto& extension : extensions)logger->trace(extension.extensionName);
		}

		void logLayers(const std::vector<vk::LayerProperties> & layers) const
		{
			logger->trace("Available VK layers: " + std::to_string(layers.size()));
			for (const auto& layer : layers)logger->trace(layer.layerName);
		}
#endif

		void flush() { if (logger) logger->flush(); }
	};
}

#endif