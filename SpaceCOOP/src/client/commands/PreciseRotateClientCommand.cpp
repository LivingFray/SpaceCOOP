#include "PreciseRotateClientCommand.h"



PreciseRotateClientCommand::PreciseRotateClientCommand() {
	name = "preciserotate";
	id = 6;
}


PreciseRotateClientCommand::~PreciseRotateClientCommand() {
}

void PreciseRotateClientCommand::execute() {
	if (!client || !client->getShip()) {
		return;
	}
	client->getShip()->setRotation(angle);
	//Tell server to run command
	ClientCommand::executeRemote();
}

sf::Packet& PreciseRotateClientCommand::packetIn(sf::Packet& packet) {
	packet = Command::packetIn(packet);
	packet >> angle;
	return packet;
}

sf::Packet& PreciseRotateClientCommand::packetOut(sf::Packet& packet) const {
	packet = Command::packetOut(packet);
	packet << angle;
	return packet;
}