#pragma once
#include "Logger.h"
#include <sstream>


////////////Declarations for static objects////////////
std::shared_ptr<spdlog::logger> Logger::RE_CoreLogger;
std::shared_ptr<spdlog::logger> Logger::RE_ClientLogger;
std::shared_ptr<spdlog::logger> Logger::RE_CoreFileLogger;
std::shared_ptr<spdlog::logger> Logger::RE_ClientFileLogger;
///////////////////////////////////////////////////////

void Logger::InitLogger()
{
	//Setting format for loggers
	//Trace is the lowest level
	spdlog::set_pattern("%^[%T] %n: (%l) %v%s");

	RE_CoreLogger = spdlog::stdout_color_mt("Engine");
	RE_CoreLogger->set_level(spdlog::level::trace);

	RE_ClientLogger = spdlog::stdout_color_mt("App");
	RE_ClientLogger->set_level(spdlog::level::trace);

	//Create a file logger that can store 3 files of 10MB of data
	//RE_FileLogger = spdlog::rotating_logger_mt("RE_FileLogger", "../logs/logfile", 10 * 1024 * 1024, 3);

	//Create a simple file logger (sink)
	RE_CoreFileLogger = spdlog::basic_logger_mt("CoreFileLogger", "log/CoreFileLog.txt", true);
	RE_CoreFileLogger->set_level(spdlog::level::trace);

	//Create a simple file logger (sink)
	RE_ClientFileLogger = spdlog::basic_logger_mt("ClientFileLogger", "log/ClientFileLog.txt",true);
	RE_ClientFileLogger->set_level(spdlog::level::trace);
}