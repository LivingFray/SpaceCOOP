#include "StrafeLeftClientCommand.h"
#include "../../shared/Console.h"


StrafeLeftClientCommand::StrafeLeftClientCommand() {
	name = "strafeleft";
}


StrafeLeftClientCommand::~StrafeLeftClientCommand() {
}

void StrafeLeftClientCommand::execute() {
	if (!client || !client->ship) {
		return;
	}
	if (held) {
		client->ship->moveRight(-1.0f);
	} else {
		client->ship->moveRight(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
