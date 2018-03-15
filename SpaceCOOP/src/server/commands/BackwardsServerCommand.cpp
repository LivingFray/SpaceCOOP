#include "BackwardsServerCommand.h"
#include "../../shared/Console.h"


BackwardsServerCommand::BackwardsServerCommand() {
	name = "backwards";
}


BackwardsServerCommand::~BackwardsServerCommand() {
}

void BackwardsServerCommand::execute() {
	if (held) {
		player->ship->moveForward(-50.0f);
	} else {
		player->ship->moveForward(0.0f);
	}
}
