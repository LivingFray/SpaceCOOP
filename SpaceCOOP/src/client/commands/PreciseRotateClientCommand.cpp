#include "PreciseRotateClientCommand.h"

double PreciseRotateClientCommand::lastRotateCommand = 0.0;
double PreciseRotateClientCommand::time = 0.0;

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
	//TODO: Limit frequency command is sent
	client->getShip()->setDesiredAngle(angle);
	//Tell server to run command
	if (time - lastRotateCommand > sendRate) {
		ClientCommand::executeRemote();
		lastRotateCommand = time;
	}
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