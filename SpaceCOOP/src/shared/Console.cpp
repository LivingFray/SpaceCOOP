#include "Console.h"
#include <iostream>


Console::Console() {
}


Console::~Console() {
}

void Console::log(std::string msg, LogLevel level) {
	//TEMP, Until proper console can be made
	switch (level) {
	case LogLevel::INFO:
		std::cout << "INFO: " << msg << "\n";
		break;
	case LogLevel::WARNING:
		std::cout << "WARNING: " << msg << "\n";
		break;
	case LogLevel::ERROR:
		std::cerr << "ERROR: " << msg << "\n";
		break;
	}
}
