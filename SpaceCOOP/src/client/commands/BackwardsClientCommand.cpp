#include "BackwardsClientCommand.h"
#include "../../shared/Console.h"


BackwardsClientCommand::BackwardsClientCommand() {
	name = "backwards";
}


BackwardsClientCommand::~BackwardsClientCommand() {
}

void BackwardsClientCommand::execute() {
	if (held) {
		client->ship->moveForward(-50.0f);
	} else {
		client->ship->moveForward(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
