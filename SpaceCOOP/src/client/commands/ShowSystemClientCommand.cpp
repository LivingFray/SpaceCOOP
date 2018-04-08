#include "ShowSystemClientCommand.h"



ShowSystemClientCommand::ShowSystemClientCommand() {
	name = "systemmap";
	id = 201;
}


ShowSystemClientCommand::~ShowSystemClientCommand() {
}

void ShowSystemClientCommand::execute() {
	if (client) {
		client->setSystemMapVisibility(held);
	}
}
