#include "StrafeLeftClientCommand.h"
#include "../../shared/Console.h"


StrafeLeftClientCommand::StrafeLeftClientCommand() {
	name = "strafeleft";
	id = 2;
}


StrafeLeftClientCommand::~StrafeLeftClientCommand() {
}

void StrafeLeftClientCommand::execute() {
	if (!client || !client->getShip()) {
		return;
	}
	if (held) {
		client->getShip()->moveRight(-1.0f);
	} else {
		client->getShip()->moveRight(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
