#include "BackwardsClientCommand.h"
#include "../../shared/Console.h"


BackwardsClientCommand::BackwardsClientCommand() {
	name = "backwards";
	id = 1;
}


BackwardsClientCommand::~BackwardsClientCommand() {
}

void BackwardsClientCommand::execute() {
	if (!client || !client->getShip()) {
		return;
	}
	if (held) {
		client->getShip()->moveForward(-1.0f);
	} else {
		client->getShip()->moveForward(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
