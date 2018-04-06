#include "ForwardsClientCommand.h"



ForwardsClientCommand::ForwardsClientCommand() {
	name = "forwards";
	id = 0;
}


ForwardsClientCommand::~ForwardsClientCommand() {
}

void ForwardsClientCommand::execute() {
	if (!client || !client->getShip()) {
		return;
	}
	//Client side extrapolation here
	if (held) {
		client->getShip()->moveForward(1.0f);
	} else {
		client->getShip()->moveForward(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
