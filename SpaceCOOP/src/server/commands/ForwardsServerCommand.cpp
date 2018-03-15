#include "ForwardsServerCommand.h"
#include "../../shared/Console.h"


ForwardsServerCommand::ForwardsServerCommand() {
	name = "forwards";
}


ForwardsServerCommand::~ForwardsServerCommand() {
}

void ForwardsServerCommand::execute() {
	if (held) {
		player->ship->moveForward(50.0f);
	} else {
		player->ship->moveForward(0.0f);
	}
}

