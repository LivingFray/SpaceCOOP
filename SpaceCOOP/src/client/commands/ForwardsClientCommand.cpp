#include "ForwardsClientCommand.h"
#include "../../shared/Console.h"



ForwardsClientCommand::ForwardsClientCommand() {
	name = "forwards";
}


ForwardsClientCommand::~ForwardsClientCommand() {
}

void ForwardsClientCommand::execute() {
	//Client side extrapolation here
	if (held) {
		client->ship->moveForward(50.0f);
	} else {
		client->ship->moveForward(0.0f);
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
