#include "ForwardsClientCommand.h"
#include "../shared/Console.h"



ForwardsClientCommand::ForwardsClientCommand() {
	name = "forwards";
}


ForwardsClientCommand::~ForwardsClientCommand() {
}

void ForwardsClientCommand::execute() {
	//Client side extrapolation here
	Console::log("Executing "+name+" " + std::to_string(held), Console::LogLevel::INFO);
	//Tell server to run command
	ClientCommand::executeRemote();
}
