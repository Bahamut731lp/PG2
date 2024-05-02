#include "Logger.h"

void Logger::debug(const std::string& message)
{
	Logger::log(Severity::DEBUG, message);
}

void Logger::info(const std::string& message)
{
	Logger::log(Severity::INFO, message);
}

void Logger::warning(const std::string& message)
{
	Logger::log(Severity::WARNING, message);
}

void Logger::error(const std::string& message)
{
	Logger::log(Severity::ERR, message);
}

void Logger::critical(const std::string& message)
{
	Logger::log(Severity::CRITICAL, message);
}
