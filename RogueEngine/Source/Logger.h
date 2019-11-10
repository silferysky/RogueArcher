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
			if (!log.GetCoreLogger())
				log.InitLogger();
			return log;
		}
		//Initializer for Logger
		static void InitLogger();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return RE_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return RE_ClientLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetCoreFileLogger() { return RE_CoreFileLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientFileLogger() { return RE_ClientFileLogger; }

		inline static std::vector<std::string>& GetConsole() { return RE_Console; }
		static void AddStringToConsole(const std::string& stringData);
		static void AssertArgs(bool args, std::string errorMsg);

	private:
		static std::vector<std::string> RE_Console;
		static std::shared_ptr<spdlog::logger> RE_CoreLogger;
		static std::shared_ptr<spdlog::logger> RE_ClientLogger;
		static std::shared_ptr<spdlog::logger> RE_CoreFileLogger;
		static std::shared_ptr<spdlog::logger> RE_ClientFileLogger;
	};

	//MACROs

	//Core Logger
#define RE_CORE_TRACE(TFirst, ...)		Logger::instance().GetCoreLogger()->trace(TFirst);		Logger::GetCoreFileLogger()->trace(TFirst);		Logger::AddStringToConsole(TFirst)
#define RE_CORE_INFO(TFirst, ...)		Logger::instance().GetCoreLogger()->info(TFirst);		Logger::GetCoreFileLogger()->info(TFirst);		Logger::AddStringToConsole(TFirst)
#define RE_CORE_WARN(TFirst, ...)		Logger::instance().GetCoreLogger()->warn(TFirst);		Logger::GetCoreFileLogger()->warn(TFirst);		Logger::AddStringToConsole(TFirst)
#define RE_CORE_ERROR(TFirst, ...)		Logger::instance().GetCoreLogger()->error(TFirst);		Logger::GetCoreFileLogger()->error(TFirst);		Logger::AddStringToConsole(TFirst)
//#define RE_CORE_FATAL(TFirst, ...)	Logger::instance().GetCoreLogger()->fatal(TFirst);		Logger::GetCoreFileLogger()->fatal(TFirst);		Logger::AddStringToConsole(TFirst)

//Client Logger
#define RE_TRACE(TFirst, ...)			Logger::instance().GetClientLogger()->trace(TFirst);	Logger::GetClientFileLogger()->trace(TFirst);	Logger::AddStringToConsole(TFirst)
#define RE_INFO(TFirst, ...)			Logger::instance().GetClientLogger()->info(TFirst);		Logger::GetClientFileLogger()->info(TFirst);	Logger::AddStringToConsole(TFirst)
#define RE_WARN(TFirst, ...)			Logger::instance().GetClientLogger()->warn(TFirst);		Logger::GetClientFileLogger()->warn(TFirst);	Logger::AddStringToConsole(TFirst)
#define RE_ERROR(TFirst, ...)			Logger::instance().GetClientLogger()->error(TFirst);	Logger::GetClientFileLogger()->error(TFirst);	Logger::AddStringToConsole(TFirst)
//#define RE_FATAL(TFirst, ...)			Logger::instance().GetClientLogger()->fatal(TFirst);	Logger::GetClientFileLogger()->fatal(TFirst);	Logger::AddStringToConsole(TFirst)

//Assert Logging
#define RE_ASSERT(args, msg)	Logger::instance().AssertArgs(args, msg);

#define CLEARSTRING(s) s.clear(); s.str("")

//The point of using instance() in the first set of commands is to ensure Logger is initialized
//Afterwards, no need to get the instance again (since it is static function anyway)
}