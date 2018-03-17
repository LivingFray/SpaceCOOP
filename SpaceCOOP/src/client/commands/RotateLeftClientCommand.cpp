#include "RotateLeftClientCommand.h"



RotateLeftClientCommand::RotateLeftClientCommand() {
	name = "rotateleft";
}


RotateLeftClientCommand::~RotateLeftClientCommand() {
}

void RotateLeftClientCommand::execute() {
	if (held) {
		client->ship->rotate(-amount);
	} else {
		client->ship->rotate(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
