#pragma once

#include <memory.h>
#include <sstream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

class Logger
{
public:

	//Initializer for Logger
	static void InitLogger();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return RE_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return RE_ClientLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetCoreFileLogger() { return RE_CoreFileLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientFileLogger() { return RE_ClientFileLogger; }

private:
	static std::shared_ptr<spdlog::logger> RE_CoreLogger;
	static std::shared_ptr<spdlog::logger> RE_ClientLogger;
	static std::shared_ptr<spdlog::logger> RE_CoreFileLogger;
	static std::shared_ptr<spdlog::logger> RE_ClientFileLogger;
};

//MACROs

//Core Logger
#define RE_CORE_TRACE(...)		Logger::GetCoreLogger()->trace(__VA_ARGS__);	Logger::GetCoreFileLogger()->trace(__VA_ARGS__)
#define RE_CORE_INFO(...)		Logger::GetCoreLogger()->info(__VA_ARGS__);		Logger::GetCoreFileLogger()->info(__VA_ARGS__)
#define RE_CORE_WARN(...)		Logger::GetCoreLogger()->warn(__VA_ARGS__);		Logger::GetCoreFileLogger()->warn(__VA_ARGS__)
#define RE_CORE_ERROR(...)		Logger::GetCoreLogger()->error(__VA_ARGS__);	Logger::GetCoreFileLogger()->error(__VA_ARGS__)
//#define RE_CORE_FATAL(...)	Logger::GetCoreLogger()->fatal(__VA_ARGS__);	Logger::GetCoreFileLogger()->fatal(__VA_ARGS__)

//Client Logger
#define RE_TRACE(...)			Logger::GetClientLogger()->trace(__VA_ARGS__);	Logger::GetClientFileLogger()->trace(__VA_ARGS__)
#define RE_INFO(...)			Logger::GetClientLogger()->info(__VA_ARGS__);	Logger::GetClientFileLogger()->info(__VA_ARGS__)
#define RE_WARN(...)			Logger::GetClientLogger()->warn(__VA_ARGS__);	Logger::GetClientFileLogger()->warn(__VA_ARGS__)
#define RE_ERROR(...)			Logger::GetClientLogger()->error(__VA_ARGS__);	Logger::GetClientFileLogger()->error(__VA_ARGS__)
//#define RE_FATAL(...)			Logger::GetClientLogger()->fatal(__VA_ARGS__);	Logger::GetClientFileLogger()->fatal(__VA_ARGS__)