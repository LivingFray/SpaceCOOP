#include "CommandLineConsole.h"
#include <iostream>

CommandLineConsole::CommandLineConsole() {
}


CommandLineConsole::~CommandLineConsole() {
}

void CommandLineConsole::log(std::string msg, LogLevel level) {
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
