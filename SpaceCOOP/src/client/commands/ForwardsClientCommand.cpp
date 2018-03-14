#include "ForwardsClientCommand.h"
#include "../../shared/Console.h"



ForwardsClientCommand::ForwardsClientCommand() {
	name = "forwards";
}


ForwardsClientCommand::~ForwardsClientCommand() {
}

void ForwardsClientCommand::execute() {
	//Client side extrapolation here
	if (held) {
		client->ship->setVelocity(sf::Vector2f(50.0, 0.0));
	} else {
		client->ship->setVelocity(sf::Vector2f(0.0, 0.0));
	}
	//Tell server to run command
	ClientCommand::executeRemote();
}
