#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Logger::RE_CoreLogger;
std::shared_ptr<spdlog::logger> Logger::RE_ClientLogger;

void Logger::InitLogger()
{
	//Setting format for both loggers
	spdlog::set_pattern("%^[%T] %n: (%l) %v%s");

	RE_CoreLogger = spdlog::stdout_color_mt("Engine");
	RE_CoreLogger->set_level(spdlog::level::trace);

	RE_ClientLogger = spdlog::stdout_color_mt("App");
	RE_ClientLogger->set_level(spdlog::level::trace);
}