#include "ShowConsoleCommand.h"



ShowConsoleCommand::ShowConsoleCommand() {
	name = "showconsole";
}


ShowConsoleCommand::~ShowConsoleCommand() {
}

void ShowConsoleCommand::execute() {
	client->showConsole();
}