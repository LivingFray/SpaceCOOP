#include "RotateLeftClientCommand.h"



RotateLeftClientCommand::RotateLeftClientCommand() {
	name = "rotateleft";
}


RotateLeftClientCommand::~RotateLeftClientCommand() {
}

void RotateLeftClientCommand::execute() {
	if (!client || !client->ship) {
		return;
	}
	if (held) {
		client->ship->rotate(-client->ship->rotateAmount);
	} else {
		client->ship->rotate(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
