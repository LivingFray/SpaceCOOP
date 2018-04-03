#include "BackwardsServerCommand.h"
#include "../../shared/Console.h"


BackwardsServerCommand::BackwardsServerCommand() {
	name = "backwards";
	id = 1;
}


BackwardsServerCommand::~BackwardsServerCommand() {
}

void BackwardsServerCommand::execute() {
	if (held) {
		player->ship->moveForward(-1.0f);
	} else {
		player->ship->moveForward(0.0f);
	}
}
