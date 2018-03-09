#include "StrafeLeftServerCommand.h"
#include "../../shared/Console.h"


StrafeLeftServerCommand::StrafeLeftServerCommand() {
	name = "strafeleft";
}


StrafeLeftServerCommand::~StrafeLeftServerCommand() {
}

void StrafeLeftServerCommand::execute() {
	if (held) {
		Console::log("DUMMY strafe left start command", Console::LogLevel::INFO);
	} else {
		Console::log("DUMMY strafe left stop command", Console::LogLevel::INFO);
	}
}
