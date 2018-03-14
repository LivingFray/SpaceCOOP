#include "ForwardsServerCommand.h"
#include "../../shared/Console.h"


ForwardsServerCommand::ForwardsServerCommand() {
	name = "forwards";
}


ForwardsServerCommand::~ForwardsServerCommand() {
}

void ForwardsServerCommand::execute() {
	if (held) {
		player->ship->setVelocity(sf::Vector2f(50.0f, 0.0f));
		Console::log("DUMMY forwards start command", Console::LogLevel::INFO);
	} else {
		player->ship->setVelocity(sf::Vector2f(0.0f, 0.0f));
		Console::log("DUMMY forwards stop command", Console::LogLevel::INFO);
	}
}

