#include "Console.h"
#include <iostream>


Console::Console() {
}


Console::~Console() {
}

void Console::logToConsole(std::string msg, LogLevel level) {
	//TEMP, Until proper console can be made
	std::string pre;
	switch (level) {
	case LogLevel::INFO:
		pre = "INFO: ";
		break;
	case LogLevel::WARNING:
		pre = "WARNING: ";
		break;
	case LogLevel::ERROR:
		pre = "ERROR: ";
		break;
	}
	msg = pre + msg + "\n";
	std::cout << msg;
}
