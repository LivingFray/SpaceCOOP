#include "ShowConsoleCommand.h"



ShowConsoleCommand::ShowConsoleCommand() {
	name = "showconsole";
	id = 200;
}


ShowConsoleCommand::~ShowConsoleCommand() {
}

void ShowConsoleCommand::execute() {
	client->showConsole();
}
