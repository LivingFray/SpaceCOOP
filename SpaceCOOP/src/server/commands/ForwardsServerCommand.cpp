#include "ForwardsServerCommand.h"
#include "../../shared/Console.h"


ForwardsServerCommand::ForwardsServerCommand() {
	name = "forwards";
}


ForwardsServerCommand::~ForwardsServerCommand() {
}

void ForwardsServerCommand::execute() {
	if (held) {
		Console::log("DUMMY forwards start command", Console::LogLevel::INFO);
	} else {
		Console::log("DUMMY forwards stop command", Console::LogLevel::INFO);
	}
}

