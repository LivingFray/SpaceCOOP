#pragma once
#include <string>

class Console {
public:
	Console();
	~Console();
	enum class LogLevel { INFO, WARNING, ERROR };
	static void logToConsole(std::string msg, LogLevel level);
};

