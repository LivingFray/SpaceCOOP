#include "BackwardsServerCommand.h"
#include "../../shared/Console.h"


BackwardsServerCommand::BackwardsServerCommand() {
	name = "backwards";
}


BackwardsServerCommand::~BackwardsServerCommand() {
}

void BackwardsServerCommand::execute() {
	if (held) {
		Console::log("DUMMY backwards start command", Console::LogLevel::INFO);
	} else {
		Console::log("DUMMY backwards stop command", Console::LogLevel::INFO);
	}
}
