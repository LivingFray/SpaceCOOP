#include "HeldCommand.h"



HeldCommand::HeldCommand() {
}


HeldCommand::~HeldCommand() {
}

void HeldCommand::parseString(std::string cmd) {
	held = cmd[0] == '+';
	execute();
}

sf::Packet& HeldCommand::packetIn(sf::Packet& packet) {
	packet = Command::packetIn(packet);
	packet >> held;
	return packet;
}

sf::Packet& HeldCommand::packetOut(sf::Packet& packet) const {
	packet = Command::packetOut(packet);
	packet << held;
	return packet;
}