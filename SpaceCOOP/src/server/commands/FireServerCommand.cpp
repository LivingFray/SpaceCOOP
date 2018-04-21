#include "FireServerCommand.h"



FireServerCommand::FireServerCommand() {
	id = 8;
	name = "fire";
}


FireServerCommand::~FireServerCommand() {
}

void FireServerCommand::execute() {
	if (player) {
		player->ship->fire();
	}
}
