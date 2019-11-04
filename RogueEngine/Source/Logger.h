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

		inline static std::vector<std::string> GetConsole() { return RE_Console; }
		static void AddStringToConsole(std::string stringData);

	private:

		static std::shared_ptr<spdlog::logger> RE_CoreLogger;
		static std::shared_ptr<spdlog::logger> RE_ClientLogger;
		static std::shared_ptr<spdlog::logger> RE_CoreFileLogger;
		static std::shared_ptr<spdlog::logger> RE_ClientFileLogger;

		static std::vector<std::string> RE_Console;
	};

	//MACROs

	//Core Logger
#define RE_CORE_TRACE(...)		Logger::instance().GetCoreLogger()->trace(__VA_ARGS__);		Logger::GetCoreFileLogger()->trace(__VA_ARGS__)
#define RE_CORE_INFO(...)		Logger::instance().GetCoreLogger()->info(__VA_ARGS__);		Logger::GetCoreFileLogger()->info(__VA_ARGS__)
#define RE_CORE_WARN(...)		Logger::instance().GetCoreLogger()->warn(__VA_ARGS__);		Logger::GetCoreFileLogger()->warn(__VA_ARGS__)
#define RE_CORE_ERROR(...)		Logger::instance().GetCoreLogger()->error(__VA_ARGS__);		Logger::GetCoreFileLogger()->error(__VA_ARGS__)
//#define RE_CORE_FATAL(...)	Logger::instance().GetCoreLogger()->fatal(__VA_ARGS__);		Logger::GetCoreFileLogger()->fatal(__VA_ARGS__)

//Client Logger
#define RE_TRACE(...)			Logger::instance().GetClientLogger()->trace(__VA_ARGS__);	Logger::GetClientFileLogger()->trace(__VA_ARGS__)
#define RE_INFO(...)			Logger::instance().GetClientLogger()->info(__VA_ARGS__);	Logger::GetClientFileLogger()->info(__VA_ARGS__)
#define RE_WARN(...)			Logger::instance().GetClientLogger()->warn(__VA_ARGS__);	Logger::GetClientFileLogger()->warn(__VA_ARGS__)
#define RE_ERROR(...)			Logger::instance().GetClientLogger()->error(__VA_ARGS__);	Logger::GetClientFileLogger()->error(__VA_ARGS__)
//#define RE_FATAL(...)			Logger::instance().GetClientLogger()->fatal(__VA_ARGS__);	Logger::GetClientFileLogger()->fatal(__VA_ARGS__)

//Assert Logging
#define RE_ASSERT(args, msg)	if(args == false) { RE_CORE_ERROR(msg);	assert(args && msg); }

#define CLEARSTRING(s) s.clear(); s.str("")

//The point of using instance() in the first set of commands is to ensure Logger is initialized
//Afterwards, no need to get the instance again (since it is static function anyway)
}