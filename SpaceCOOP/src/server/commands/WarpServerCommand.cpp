#include "WarpServerCommand.h"


WarpServerCommand::WarpServerCommand() {
	name = "warp";
	id = 7;
}


WarpServerCommand::~WarpServerCommand() {
}

void WarpServerCommand::execute() {
	player->warp(destination);
}


sf::Packet& WarpServerCommand::packetIn(sf::Packet& packet) {
	packet = Command::packetIn(packet);
	packet >> destination;
	return packet;
}

sf::Packet& WarpServerCommand::packetOut(sf::Packet& packet) const {
	packet = Command::packetOut(packet);
	packet << destination;
	return packet;
}