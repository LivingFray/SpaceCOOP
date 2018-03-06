#include "StrafeRightServerCommand.h"
#include "../shared/Console.h"


StrafeRightServerCommand::StrafeRightServerCommand() {
	name = "straferight";
}


StrafeRightServerCommand::~StrafeRightServerCommand() {
}

void StrafeRightServerCommand::execute() {
	if (held) {
		Console::log("DUMMY strafe right start command", Console::LogLevel::INFO);
	} else {
		Console::log("DUMMY strafe right stop command", Console::LogLevel::INFO);
	}
}
