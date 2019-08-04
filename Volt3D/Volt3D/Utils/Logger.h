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
	class VOLT3D_DLL Logger
	{
	private:
		std::shared_ptr<spdlog::logger> logger;
		std::shared_ptr<spdlog::logger> console;

		Logger() : logger( nullptr ), console( nullptr ) {}
		~Logger() {
			logger->flush();
			if (console) console->flush();
		}

	public:
		DELETE_COPY_AND_COPY_ASSIGN_CONSTRUCTOR( Logger );
		DEFAULT_MOVE_CONSTRUCTORS( Logger );

		static Logger& getInstance()
		{
			static Logger instance;
			return instance;
		}

		bool init( const std::wstring& filePath, const std::wstring& fileName )
		{
			if (logger) return false;

			std::filesystem::path loggerPath( filePath.empty() ? std::filesystem::current_path().c_str() : filePath );
			loggerPath /= std::filesystem::path( fileName.empty() ? L"log" : fileName );

			if (std::filesystem::exists( loggerPath )) std::filesystem::remove( loggerPath );

			std::wifstream ifs( loggerPath );
			ifs.close();

			logger = spdlog::basic_logger_mt( "Log", loggerPath.c_str() );
			logger->set_pattern( "[%l] [%c] %v" );
			logger->set_level( spdlog::level::trace );

#ifdef BUILD_DEBUG
			logger->trace( "Trace test" );
			logger->debug( "Debug test" );
			logger->info( "Info test" );
			logger->warn( "Warning test" );
			logger->error( "Error test" );
			logger->critical( "Critical test" );
#endif

			logger->trace( (std::wstring( L"Created log file at: " ) + loggerPath.wstring()).c_str() );
			logger->flush();

			return true;
		}

		bool initConsole()
		{
			if (console) return false;
			console = spdlog::stdout_color_mt( "Console" );
			console->set_pattern( "%^[%l] [%c] %v%$" );
			console->set_level( spdlog::level::trace );

#ifdef BUILD_DEBUG
			console->trace( "Trace test" );
			console->debug( "Debug test" );
			console->info( "Info test" );
			console->warn( "Warning test" );
			console->error( "Error test" );
			console->critical( "Critical test" );
#endif

			return true;
		}

		void setPattern( const std::string& pattern ) const { if (logger) logger->set_pattern( pattern.c_str() ); }
		void setConsolePattern( const std::string& pattern ) const { if (console) console->set_pattern( pattern.c_str() ); }

		// ==============================
		inline void trace( const char* msg ) const { if (logger) logger->trace( msg ); traceConsole( msg ); }
		inline void trace( const std::string& msg ) const { if (logger) logger->trace( msg ); traceConsole( msg.c_str() ); }
		inline void trace( const wchar_t* msg ) const { if (logger) logger->trace( msg ); traceConsole( msg ); }
		inline void trace( const std::wstring& msg ) const { if (logger) logger->trace( msg.c_str() ); traceConsole( msg.c_str() ); }

		inline void debug( const char* msg ) const { if (logger) logger->debug( msg ); debugConsole( msg ); }
		inline void debug( const std::string& msg ) const { if (logger) logger->debug( msg ); debugConsole( msg.c_str() ); }
		inline void debug( const wchar_t* msg ) const { if (logger) logger->debug( msg ); debugConsole( msg ); }
		inline void debug( const std::wstring& msg ) const { if (logger) logger->debug( msg.c_str() ); debugConsole( msg.c_str() ); }

		inline void info( const char* msg ) const { if (logger) logger->info( msg ); infoConsole( msg ); }
		inline void info( const std::string& msg ) const { if (logger) logger->info( msg ); infoConsole( msg.c_str() ); }
		inline void info( const wchar_t* msg ) const { if (logger) logger->info( msg ); infoConsole( msg ); }
		inline void info( const std::wstring& msg ) const { if (logger) logger->info( msg.c_str() ); infoConsole( msg.c_str() ); }

		inline void warn( const char* msg ) const { if (logger) logger->warn( msg ); warnConsole( msg ); }
		inline void warn( const std::string& msg ) const { if (logger) logger->warn( msg ); warnConsole( msg.c_str() ); }
		inline void warn( const wchar_t* msg ) const { if (logger) logger->warn( msg ); warnConsole( msg ); }
		inline void warn( const std::wstring& msg ) const { if (logger) logger->warn( msg.c_str() ); warnConsole( msg.c_str() ); }

		inline void error( const char* msg ) const { if (logger) logger->error( msg ); errorConsole( msg ); }
		inline void error( const std::string& msg ) const { if (logger) logger->error( msg ); errorConsole( msg.c_str() ); }
		inline void error( const wchar_t* msg ) const { if (logger) logger->error( msg ); errorConsole( msg ); }
		inline void error( const std::wstring& msg ) const { if (logger) logger->error( msg.c_str() ); errorConsole( msg.c_str() ); }

		inline void critical( const char* msg ) const { if (logger) logger->critical( msg ); criticalConsole( msg ); }
		inline void critical( const std::string& msg ) const { if (logger) logger->critical( msg ); criticalConsole( msg.c_str() ); }
		inline void critical( const wchar_t* msg ) const { if (logger) logger->critical( msg ); criticalConsole( msg ); }
		inline void critical( const std::wstring& msg ) const { if (logger) logger->critical( msg.c_str() ); criticalConsole( msg.c_str() ); }
		// ==============================


		// ==============================
		template<typename... Args> inline void trace( const char* fmt, const Args& ... args ) { if (logger) logger->trace( fmt, args ... ); traceConsole( fmt, args ... ); }
		template<typename... Args> inline void trace( const std::string& fmt, const Args& ... args ) { if (logger) logger->trace( fmt.c_str(), args ... ); traceConsole( fmt.c_str(), args ... ); }
		template<typename... Args> inline void trace( const wchar_t* fmt, const Args& ... args ) { if (logger) logger->trace( fmt, args ... ); traceConsole( fmt, args ... ); }
		template<typename... Args> inline void trace( const std::wstring& fmt, const Args& ... args ) { if (logger) logger->trace( fmt.c_str(), args ... ); traceConsole( fmt.c_str(), args ... ); }

		template<typename... Args> inline void debug( const char* fmt, const Args& ... args ) { if (logger) logger->debug( fmt, args ... ); debugConsole( fmt, args ... ); }
		template<typename... Args> inline void debug( const std::string& fmt, const Args& ... args ) { if (logger) logger->debug( fmt.c_str(), args ... ); debugConsole( fmt.c_str(), args ... ); }
		template<typename... Args> inline void debug( const wchar_t* fmt, const Args& ... args ) { if (logger) logger->debug( fmt, args ... ); debugConsole( fmt, args ... ); }
		template<typename... Args> inline void debug( const std::wstring& fmt, const Args& ... args ) { if (logger) logger->debug( fmt.c_str(), args ... ); debugConsole( fmt.c_str(), args ... ); }
		
		template<typename... Args> inline void info( const char* fmt, const Args& ... args ) { if (logger) logger->info( fmt, args ... ); infoConsole( fmt, args ... ); }
		template<typename... Args> inline void info( const std::string& fmt, const Args& ... args ) { if (logger) logger->info( fmt, args.c_str() ... ); infoConsole( fmt.c_str(), args ... ); }
		template<typename... Args> inline void info( const wchar_t* fmt, const Args& ... args ) { if (logger) logger->info( fmt, args ... ); infoConsole( fmt, args ... ); }
		template<typename... Args> inline void info( const std::wstring& fmt, const Args& ... args ) { if (logger) logger->info( fmt.c_str(), args ... ); infoConsole( fmt.c_str(), args ... ); }
		
		template<typename... Args> inline void warn( const char* fmt, const Args& ... args ) { if (logger) logger->warn( fmt, args ... ); warnConsole( fmt, args ... ); }
		template<typename... Args> inline void warn( const std::string& fmt, const Args& ... args ) { if (logger) logger->warn( fmt, args.c_str() ... ); warnConsole( fmt.c_str(), args ... ); }
		template<typename... Args> inline void warn( const wchar_t* fmt, const Args& ... args ) { if (logger) logger->warn( fmt, args ... ); warnConsole( fmt, args ... ); }
		template<typename... Args> inline void warn( const std::wstring& fmt, const Args& ... args ) { if (logger) logger->warn( fmt.c_str(), args ... ); warnConsole( fmt.c_str(), args ... ); }
		
		template<typename... Args> inline void error( const char* fmt, const Args& ... args ) { if (logger) logger->error( fmt, args ... ); errorConsole( fmt, args ... ); }
		template<typename... Args> inline void error( const std::string& fmt, const Args& ... args ) { if (logger) logger->error( fmt.c_str(), args ... ); errorConsole( fmt.c_str(), args ... ); }
		template<typename... Args> inline void error( const wchar_t* fmt, const Args& ... args ) { if (logger) logger->error( fmt, args ... ); errorConsole( fmt, args ... ); }
		template<typename... Args> inline void error( const std::wstring& fmt, const Args& ... args ) { if (logger) logger->error( fmt.c_str(), args ... ); errorConsole( fmt.c_str(), args ... ); }
		
		template<typename... Args> inline void critical( const char* fmt, const Args& ... args ) { if (logger) logger->critical( fmt, args ... ); criticalConsole( fmt, args ... ); }
		template<typename... Args> inline void critical( const std::string& fmt, const Args& ... args ) { if (logger) logger->critical(fmt.c_str(), args ...); criticalConsole( fmt.c_str(), args ... ); }
		template<typename... Args> inline void critical( const wchar_t* fmt, const Args& ... args ) { if (logger) logger->critical( fmt, args ... ); criticalConsole( fmt, args ... ); }
		template<typename... Args> inline void critical( const std::wstring& fmt, const Args& ... args ) { if (logger) logger->critical( fmt.c_str(), args ... ); criticalConsole( fmt.c_str(), args ... ); }
		// ==============================


		// ==============================
		inline void traceConsole( const char* msg ) const { if (console) console->trace( msg ); }
		inline void traceConsole( const std::string& msg ) const { if (console) console->trace( msg.c_str() ); }
		inline void traceConsole( const wchar_t* msg ) const { if (console) console->trace( msg ); }
		inline void traceConsole( const std::wstring& msg ) const { if (console) console->trace( msg.c_str() ); }

		inline void debugConsole( const char* msg ) const { if (console) console->debug( msg ); }
		inline void debugConsole( const std::string& msg ) const { if (console) console->debug( msg.c_str() ); }
		inline void debugConsole( const wchar_t* msg ) const { if (console) console->debug( msg ); }
		inline void debugConsole( const std::wstring& msg ) const { if (console) console->debug( msg.c_str() ); }

		inline void infoConsole( const char* msg ) const { if (console) console->info( msg ); }
		inline void infoConsole( const std::string& msg ) const { if (console) console->info( msg.c_str() ); }
		inline void infoConsole( const wchar_t* msg ) const { if (console) console->info( msg ); }
		inline void infoConsole( const std::wstring& msg ) const { if (console) console->info( msg.c_str() ); }

		inline void warnConsole( const char* msg ) const { if (console) console->warn( msg ); }
		inline void warnConsole( const std::string& msg ) const { if (console) console->warn( msg.c_str() ); }
		inline void warnConsole( const wchar_t* msg ) const { if (console) console->warn( msg ); }
		inline void warnConsole( const std::wstring& msg ) const { if (console) console->warn( msg.c_str() ); }

		inline void errorConsole( const char* msg ) const { if (console) console->error( msg ); }
		inline void errorConsole( const std::string& msg ) const { if (console) console->error( msg.c_str() ); }
		inline void errorConsole( const wchar_t* msg ) const { if (console) console->error( msg ); }
		inline void errorConsole( const std::wstring& msg ) const { if (console) console->error( msg.c_str() ); }

		inline void criticalConsole( const char* msg ) const { if (console) console->critical( msg ); }
		inline void criticalConsole( const std::string& msg ) const { if (console) console->critical( msg.c_str() ); }
		inline void criticalConsole( const wchar_t* msg ) const { if (console) console->critical( msg ); }
		inline void criticalConsole( const std::wstring& msg ) const { if (console) console->critical( msg.c_str() ); }
		// ==============================


		// ==============================
		template<typename... Args> inline void traceConsole( const char* fmt, const Args& ... args ) const { if (console) console->trace( fmt, args ... ); }
		template<typename... Args> inline void traceConsole( const std::string& fmt, const Args& ... args ) const { if (console) console->trace( fmt.c_str(), args ... ); }
		template<typename... Args> inline void traceConsole( const wchar_t* fmt, const Args& ... args ) const { if (console) console->trace( fmt, args ... ); }
		template<typename... Args> inline void traceConsole( const std::wstring& fmt, const Args& ... args ) const { if (console) console->trace( fmt.c_str(), args ... ); }

		template<typename... Args> inline void debugConsole( const char* fmt, const Args& ... args ) const { if (console) console->debug( fmt, args ... ); }
		template<typename... Args> inline void debugConsole( const std::string& fmt, const Args& ... args ) const { if (console) console->debug( fmt.c_str(), args ... ); }
		template<typename... Args> inline void debugConsole( const wchar_t* fmt, const Args& ... args ) const { if (console) console->debug( fmt, args ... ); }
		template<typename... Args> inline void debugConsole( const std::wstring& fmt, const Args& ... args ) const { if (console) console->debug( fmt.c_str(), args ... ); }

		template<typename... Args> inline void infoConsole( const char* fmt, const Args& ... args ) const { if (console) console->info( fmt, args ... ); }
		template<typename... Args> inline void infoConsole( const std::string& fmt, const Args& ... args ) const { if (console) console->info( fmt.c_str(), args ... ); }
		template<typename... Args> inline void infoConsole( const wchar_t* fmt, const Args& ... args ) const { if (console) console->info( fmt, args ... ); }
		template<typename... Args> inline void infoConsole( const std::wstring& fmt, const Args& ... args ) const { if (console) console->info( fmt.c_str(), args ... ); }

		template<typename... Args> inline void warnConsole( const char* fmt, const Args& ... args ) const { if (console) console->warn( fmt, args ... ); }
		template<typename... Args> inline void warnConsole( const std::string& fmt, const Args& ... args ) const { if (console) console->warn( fmt.c_str(), args ... ); }
		template<typename... Args> inline void warnConsole( const wchar_t* fmt, const Args& ... args ) const { if (console) console->warn( fmt, args ... ); }
		template<typename... Args> inline void warnConsole( const std::wstring& fmt, const Args& ... args ) const { if (console) console->warn( fmt.c_str(), args ... ); }

		template<typename... Args> inline void errorConsole( const char* fmt, const Args& ... args ) const { if (console) console->error( fmt, args ... ); }
		template<typename... Args> inline void errorConsole( const std::string& fmt, const Args& ... args ) const { if (console) console->error( fmt.c_str(), args ... ); }
		template<typename... Args> inline void errorConsole( const wchar_t* fmt, const Args& ... args ) const { if (console) console->error( fmt, args ... ); }
		template<typename... Args> inline void errorConsole( const std::wstring& fmt, const Args& ... args ) const { if (console) console->error( fmt.c_str(), args ... ); }

		template<typename... Args> inline void criticalConsole( const char* fmt, const Args& ... args ) const { if (console) console->critical( fmt, args ... ); }
		template<typename... Args> inline void criticalConsole( const std::string& fmt, const Args& ... args ) const { if (console) console->critical( fmt.c_str(), args ... ); }
		template<typename... Args> inline void criticalConsole( const wchar_t* fmt, const Args& ... args ) const { if (console) console->critical( fmt, args ... ); }
		template<typename... Args> inline void criticalConsole( const std::wstring& fmt, const Args& ... args ) const { if (console) console->critical( fmt.c_str(), args ... ); }
		// ==============================
		
#ifdef BUILD_DEBUG
		void logExtensions( const std::vector<vk::ExtensionProperties>& extensions ) const
		{
			logger->trace( "Available VK extensions: " + std::to_string( extensions.size() ) );
			for (const auto& extension : extensions)logger->trace( extension.extensionName );
		}

		void logLayers( const std::vector<vk::LayerProperties>& layers ) const
		{
			logger->trace( "Available VK layers: " + std::to_string( layers.size() ) );
			for (const auto& layer : layers)logger->trace( layer.layerName );
		}
#endif

		void flush() { if (logger) logger->flush(); }
	};
}

#endif