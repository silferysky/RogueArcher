#pragma once
#include "Precompiled.h"
#include "Logger.h"


namespace Rogue
{
	////////////Declarations for static objects////////////
	std::shared_ptr<spdlog::logger> Logger::RE_CoreFileLogger;
	std::shared_ptr<spdlog::logger> Logger::RE_ClientFileLogger;
	std::vector<std::string> Logger::RE_Console;
	///////////////////////////////////////////////////////

	void Logger::InitLogger()
	{
		//Setting format for loggers
		//Trace is the lowest level
		spdlog::set_pattern("%^[%T] %n: (%l) %v%s");

		/*RE_CoreLogger = spdlog::stdout_color_mt("Engine");
		RE_CoreLogger->set_level(spdlog::level::trace);

		RE_ClientLogger = spdlog::stdout_color_mt("App");
		RE_ClientLogger->set_level(spdlog::level::trace);*/

		//Create a file logger that can store 3 files of 10MB of data
		//RE_FileLogger = spdlog::rotating_logger_mt("RE_FileLogger", "../logs/logfile", 10 * 1024 * 1024, 3);

		//Create a simple file logger (sink)
		RE_CoreFileLogger = spdlog::basic_logger_mt("CoreFileLogger", "log/CoreFileLog.txt", true);
		RE_CoreFileLogger->set_level(spdlog::level::trace);

		//Create a simple file logger (sink)
		RE_ClientFileLogger = spdlog::basic_logger_mt("ClientFileLogger", "log/ClientFileLog.txt", true);
		RE_ClientFileLogger->set_level(spdlog::level::trace);
	}

	void Logger::AddStringToConsole(std::string_view stringData)
	{
		RE_Console.emplace(RE_Console.begin(), stringData);
		if (RE_Console.size() > CONSOLE_SIZE)
		{
			RE_Console.pop_back();
		}
	}

	void Logger::AssertArgs(bool args, std::string_view errorMsg)
	{
		if (!args)
		{
			RE_CORE_ERROR(errorMsg);

#if defined(DEBUG) | defined(_DEBUG)
			assert(args && std::string(errorMsg).data());
#endif

		}
	}
}