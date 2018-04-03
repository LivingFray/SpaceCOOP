#include "PreciseRotateServerCommand.h"



PreciseRotateServerCommand::PreciseRotateServerCommand() {
	name = "preciserotate";
	id = 6;
}


PreciseRotateServerCommand::~PreciseRotateServerCommand() {
}

void PreciseRotateServerCommand::execute() {
	if (!player || !player->ship) {
		return;
	}
	player->ship->setRotation(angle);
}

sf::Packet& PreciseRotateServerCommand::packetIn(sf::Packet& packet) {
	packet = Command::packetIn(packet);
	packet >> angle;
	return packet;
}

sf::Packet& PreciseRotateServerCommand::packetOut(sf::Packet& packet) const {
	packet = Command::packetOut(packet);
	packet << angle;
	return packet;
}