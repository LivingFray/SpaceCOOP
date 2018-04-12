#include "WarpClientCommand.h"



WarpClientCommand::WarpClientCommand() {
	name = "warp";
	id = 7;
}


WarpClientCommand::~WarpClientCommand() {
}

void WarpClientCommand::parseString(std::string cmd) {
	if (cmd.size() < 6 || cmd[4] != ' ') {
		client->console.log("Command missing destination argument", GraphicalConsole::LogLevel::ERROR);
		return; //TODO: Failure message
	}
	try {
		int sys = stoi(cmd.substr(5));
		destination = sys;
		execute();
	} catch (std::exception e) {
		client->console.log("Destination is not a number", GraphicalConsole::LogLevel::ERROR);
		return;
	}
}

void WarpClientCommand::execute() {
	ClientCommand::executeRemote();
}


sf::Packet& WarpClientCommand::packetIn(sf::Packet& packet) {
	packet = Command::packetIn(packet);
	packet >> destination;
	return packet;
}

sf::Packet& WarpClientCommand::packetOut(sf::Packet& packet) const {
	packet = Command::packetOut(packet);
	packet << destination;
	return packet;
}