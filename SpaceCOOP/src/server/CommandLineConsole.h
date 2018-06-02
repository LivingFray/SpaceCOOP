#pragma once
#include "../shared/Console.h"
#include <string>
class CommandLineConsole :
	public Console{
public:
	CommandLineConsole();
	~CommandLineConsole();

	void log(std::string msg, LogLevel level);
};

