#include "StrafeLeftServerCommand.h"
#include "../../shared/Console.h"


StrafeLeftServerCommand::StrafeLeftServerCommand() {
	name = "strafeleft";
}


StrafeLeftServerCommand::~StrafeLeftServerCommand() {
}

void StrafeLeftServerCommand::execute() {
	if (held) {
		player->ship->moveRight(-50.0f);
	} else {
		player->ship->moveRight(0.0f);
	}
}
