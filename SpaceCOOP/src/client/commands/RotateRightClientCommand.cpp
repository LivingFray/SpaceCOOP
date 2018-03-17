#include "RotateRightClientCommand.h"



RotateRightClientCommand::RotateRightClientCommand() {
	name = "rotateright";
}


RotateRightClientCommand::~RotateRightClientCommand() {
}

void RotateRightClientCommand::execute() {
	if (held) {
		client->ship->rotate(amount);
	} else {
		client->ship->rotate(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
