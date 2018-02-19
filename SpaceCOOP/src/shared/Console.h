#pragma once
#include <string>

class Console {
public:
	Console();
	~Console();
	enum class LogLevel { INFO, WARNING, ERROR };
	static void log(std::string msg, LogLevel level);
};

