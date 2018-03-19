#include "StrafeLeftClientCommand.h"
#include "../../shared/Console.h"


StrafeLeftClientCommand::StrafeLeftClientCommand() {
	name = "strafeleft";
}


StrafeLeftClientCommand::~StrafeLeftClientCommand() {
}

void StrafeLeftClientCommand::execute() {
	if (held) {
		client->ship->moveRight(-50.0f);
	} else {
		client->ship->moveRight(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}