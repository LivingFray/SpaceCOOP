#include "StrafeRightClientCommand.h"
#include "../../shared/Console.h"


StrafeRightClientCommand::StrafeRightClientCommand() {
	name = "straferight";
	id = 3;
}


StrafeRightClientCommand::~StrafeRightClientCommand() {
}

void StrafeRightClientCommand::execute() {
	if (!client || !client->ship) {
		return;
	}
	if (held) {
		client->ship->moveRight(1.0f);
	} else {
		client->ship->moveRight(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
