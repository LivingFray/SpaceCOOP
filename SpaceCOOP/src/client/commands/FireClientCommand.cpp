#include "FireClientCommand.h"



FireClientCommand::FireClientCommand() {
	id = 8;
	name = "fire";
}


FireClientCommand::~FireClientCommand() {
}

void FireClientCommand::execute() {
	if (client) {
		client->getShip()->fire();
		ClientCommand::executeRemote();
	}
}
