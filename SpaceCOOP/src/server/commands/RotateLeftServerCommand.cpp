#include "RotateLeftServerCommand.h"



RotateLeftServerCommand::RotateLeftServerCommand() {
	name = "rotateleft";
}


RotateLeftServerCommand::~RotateLeftServerCommand() {
}

void RotateLeftServerCommand::execute() {
	if (held) {
		player->ship->rotate(-amount);
	} else {
		player->ship->rotate(0.0f);
	}
}