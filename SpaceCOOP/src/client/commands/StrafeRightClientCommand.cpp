#include "StrafeRightClientCommand.h"
#include "../../shared/Console.h"


StrafeRightClientCommand::StrafeRightClientCommand() {
	name = "straferight";
}


StrafeRightClientCommand::~StrafeRightClientCommand() {
}

void StrafeRightClientCommand::execute() {
	if (held) {
		client->ship->moveForward(50.0f);
	} else {
		client->ship->moveRight(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
