#include "BackwardsClientCommand.h"
#include "../../shared/Console.h"


BackwardsClientCommand::BackwardsClientCommand() {
	name = "backwards";
	id = 1;
}


BackwardsClientCommand::~BackwardsClientCommand() {
}

void BackwardsClientCommand::execute() {
	if (!client || !client->ship) {
		return;
	}
	if (held) {
		client->ship->moveForward(-1.0f);
	} else {
		client->ship->moveForward(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
