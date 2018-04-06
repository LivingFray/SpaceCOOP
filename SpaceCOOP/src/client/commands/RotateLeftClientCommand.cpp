#include "RotateLeftClientCommand.h"



RotateLeftClientCommand::RotateLeftClientCommand() {
	name = "rotateleft";
	id = 4;
}


RotateLeftClientCommand::~RotateLeftClientCommand() {
}

void RotateLeftClientCommand::execute() {
	if (!client || !client->getShip()) {
		return;
	}
	if (held) {
		client->getShip()->rotate(-client->getShip()->rotateAmount);
	} else {
		client->getShip()->rotate(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
