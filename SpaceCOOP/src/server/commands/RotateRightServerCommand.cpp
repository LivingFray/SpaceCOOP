#include "RotateRightServerCommand.h"



RotateRightServerCommand::RotateRightServerCommand() {
	name = "rotateright";
	id = 5;
}


RotateRightServerCommand::~RotateRightServerCommand() {
}

void RotateRightServerCommand::execute() {
	if (held) {
		player->ship->rotate(player->ship->rotateAmount);
	} else {
		player->ship->rotate(0.0f);
	}
}
