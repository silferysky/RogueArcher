/* Start Header ************************************************************************/
/*!
\file           Logger.cpp
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

	void Logger::AssertArgs(bool args, const char* errorMsg)
	{
		if (!args)
		{
			RE_CORE_ERROR(errorMsg);

			//#if defined (_DEBUG) | defined (DEBUG)
			//assert(args && errorMsg);
		}
	}
}