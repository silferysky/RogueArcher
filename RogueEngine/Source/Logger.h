/* Start Header ************************************************************************/
/*!
\file           Logger.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Logger

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <vector>
#include <string>

#define CONSOLE_SIZE 100

namespace Rogue
{
	class Logger
	{
	public:

		//Singleton for Logger
		static Logger& instance()
		{
			static Logger log;
			if (!log.GetCoreFileLogger())
				log.InitLogger();
			return log;
		}
		//Initializer for Logger
		static void InitLogger();

		inline static std::shared_ptr<spdlog::logger>& GetCoreFileLogger() { return RE_CoreFileLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientFileLogger() { return RE_ClientFileLogger; }

		inline static std::vector<std::string>& GetConsole() { return RE_Console; }
		static void AddStringToConsole(std::string_view stringData);
		static void AssertArgs(bool args, const char* errorMsg);

	private:
		static std::vector<std::string> RE_Console;
		static std::shared_ptr<spdlog::logger> RE_CoreFileLogger;
		static std::shared_ptr<spdlog::logger> RE_ClientFileLogger;
	};

//MACROs

//Core Logger
#if ENABLE_LOGGER
	#define RE_CORE_TRACE(TFirst, ...)		Logger::instance().GetCoreFileLogger()->trace(TFirst);		Logger::AddStringToConsole(TFirst)

	#define RE_CORE_INFO(TFirst, ...)		Logger::instance().GetCoreFileLogger()->info(TFirst);		Logger::AddStringToConsole(TFirst)

	#define RE_CORE_WARN(TFirst, ...)		Logger::instance().GetCoreFileLogger()->warn(TFirst);		Logger::AddStringToConsole(TFirst)

	#define RE_CORE_ERROR(TFirst, ...)			Logger::instance().GetCoreFileLogger()->error(TFirst);		Logger::AddStringToConsole(TFirst)

	//#define RE_CORE_FATAL(TFirst, ...)	Logger::instance().GetCoreFileLogger()->fatal(TFirst);		Logger::AddStringToConsole(TFirst)

//Client Logger
	#define RE_TRACE(TFirst, ...)		Logger::instance().GetClientFileLogger()->trace(TFirst);	Logger::AddStringToConsole(TFirst)

	#define RE_INFO(TFirst, ...)		Logger::instance().GetClientFileLogger()->info(TFirst);		Logger::AddStringToConsole(TFirst)

	#define RE_WARN(TFirst, ...)		Logger::instance().GetClientFileLogger()->warn(TFirst);		Logger::AddStringToConsole(TFirst)

	#define RE_ERROR(TFirst, ...)		Logger::instance().GetClientFileLogger()->error(TFirst);	Logger::AddStringToConsole(TFirst)

	//#define RE_FATAL(TFirst, ...)			Logger::instance().GetClientFileLogger()->fatal(TFirst);	Logger::AddStringToConsole(TFirst)

#else // If logger is disabled. 
	#define RE_CORE_TRACE(TFirst, ...)
	#define RE_CORE_INFO(TFirst, ...)
	#define RE_CORE_WARN(TFirst, ...)
	#define RE_CORE_ERROR(TFirst, ...)
	#define RE_TRACE(TFirst, ...)
	#define RE_INFO(TFirst, ...)
	#define RE_WARN(TFirst, ...)
	#define RE_ERROR(TFirst, ...)
#endif

//Assert Logging
//Uncommented version does not provide a readable message, but commented version (if statement) will run in both modes
#if ENABLE_ASSERT
	#define RE_ASSERT(args, msg)	Logger::instance().AssertArgs(args, msg); if(args == 0) { RE_CORE_ERROR(msg);	assert(args && msg); }
#else
	#define RE_ASSERT(args, msg)
#endif

#define CLEARSTRING(s) s.clear(); s.str("")

//The point of using instance() in the first set of commands is to ensure Logger is initialized
//Afterwards, no need to get the instance again (since it is static function anyway)
}