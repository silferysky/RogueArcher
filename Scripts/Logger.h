#pragma once

#include <memory.h>
#include "spdlog/spdlog.h"

class Logger
{
public:

	//Initializer for Logger
	static void InitLogger();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return RE_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return RE_ClientLogger; }

private:
	static std::shared_ptr<spdlog::logger> RE_CoreLogger;
	static std::shared_ptr<spdlog::logger> RE_ClientLogger;
};

//MACROs

//Core Logger
#define RE_CORE_TRACE(...)		Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define RE_CORE_INFO(...)		Logger::GetCoreLogger()->info(__VA_ARGS__)
#define RE_CORE_WARN(...)		Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define RE_CORE_ERROR(...)		Logger::GetCoreLogger()->error(__VA_ARGS__)
#define RE_CORE_FATAL(...)		Logger::GetCoreLogger()->fatal(__VA_ARGS__)

//Client Logger
#define RE_TRACE(...)	Logger::GetClientLogger()->trace(__VA_ARGS__)
#define RE_INFO(...)		Logger::GetClientLogger()->info(__VA_ARGS__)
#define RE_WARN(...)		Logger::GetClientLogger()->warn(__VA_ARGS__)
#define RE_ERROR(...)	Logger::GetClientLogger()->error(__VA_ARGS__)
#define RE_FATAL(...)	Logger::GetClientLogger()->fatal(__VA_ARGS__)