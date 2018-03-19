#include "ToggleConsoleCommand.h"



ToggleConsoleCommand::ToggleConsoleCommand() {
	name = "toggleconsole";
}


ToggleConsoleCommand::~ToggleConsoleCommand() {
}

void ToggleConsoleCommand::execute() {
	client->consoleVisible = !client->consoleVisible;
}
