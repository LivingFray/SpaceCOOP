#include "ForwardsClientCommand.h"



ForwardsClientCommand::ForwardsClientCommand() {
	name = "forwards";
}


ForwardsClientCommand::~ForwardsClientCommand() {
}

void ForwardsClientCommand::execute() {
	if (!client || !client->ship) {
		return;
	}
	//Client side extrapolation here
	if (held) {
		client->ship->moveForward(1.0f);
	} else {
		client->ship->moveForward(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
