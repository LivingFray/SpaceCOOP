#include "ForwardsServerCommand.h"
#include "../../shared/Console.h"


ForwardsServerCommand::ForwardsServerCommand() {
	name = "forwards";
	id = 0;
}


ForwardsServerCommand::~ForwardsServerCommand() {
}

void ForwardsServerCommand::execute() {
	if (held) {
		player->ship->moveForward(1.0f);
	} else {
		player->ship->moveForward(0.0f);
	}
}

