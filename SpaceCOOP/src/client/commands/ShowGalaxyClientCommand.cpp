#include "ShowGalaxyClientCommand.h"



ShowGalaxyClientCommand::ShowGalaxyClientCommand() {
	name = "togglegalaxy";
	id = 202;
}


ShowGalaxyClientCommand::~ShowGalaxyClientCommand() {
}

void ShowGalaxyClientCommand::execute() {
	client->toggleGalaxyMapVisible();
}
