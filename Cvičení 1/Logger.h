#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>

enum class Severity {
    DEBUG,
    INFO,
    WARNING,
    ERR,
    CRITICAL
};

class Logger
{
public:
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
    static void critical(const std::string& message);

private:
    static void log(Severity severity, const std::string& message) {
        std::time_t now = std::time(nullptr);
        std::tm tm_now;
        localtime_s(&tm_now, &now);

        std::cout << "[" << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S") << "] ";

        switch (severity) {
        case Severity::DEBUG:
            std::cout << "[DEBUG] ";
            break;
        case Severity::INFO:
            std::cout << "[INFO] ";
            break;
        case Severity::WARNING:
            std::cout << "[WARNING] ";
            break;
        case Severity::ERR:
            std::cout << "[ERROR] ";
            break;
        case Severity::CRITICAL:
            std::cout << "[CRITICAL] ";
            break;
        }

        std::cout << message << std::endl;
    }
};

