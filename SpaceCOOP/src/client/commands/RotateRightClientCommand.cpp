#include "RotateRightClientCommand.h"



RotateRightClientCommand::RotateRightClientCommand() {
	name = "rotateright";
}


RotateRightClientCommand::~RotateRightClientCommand() {
}

void RotateRightClientCommand::execute() {
	if (!client || !client->ship) {
		return;
	}
	if (held) {
		client->ship->rotate(client->ship->rotateAmount);
	} else {
		client->ship->rotate(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
