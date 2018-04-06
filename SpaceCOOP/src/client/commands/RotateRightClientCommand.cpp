#include "RotateRightClientCommand.h"



RotateRightClientCommand::RotateRightClientCommand() {
	name = "rotateright";
	id = 5;
}


RotateRightClientCommand::~RotateRightClientCommand() {
}

void RotateRightClientCommand::execute() {
	if (!client || !client->getShip()) {
		return;
	}
	if (held) {
		client->getShip()->rotate(client->getShip()->rotateAmount);
	} else {
		client->getShip()->rotate(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
