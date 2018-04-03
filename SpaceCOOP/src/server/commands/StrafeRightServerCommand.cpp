#include "StrafeRightServerCommand.h"
#include "../../shared/Console.h"


StrafeRightServerCommand::StrafeRightServerCommand() {
	name = "straferight";
	id = 3;
}


StrafeRightServerCommand::~StrafeRightServerCommand() {
}

void StrafeRightServerCommand::execute() {
	if (held) {
		player->ship->moveRight(1.0f);
	} else {
		player->ship->moveRight(0.0f);
	}
}
