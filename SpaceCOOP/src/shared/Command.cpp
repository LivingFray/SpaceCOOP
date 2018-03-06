#include "Command.h"



Command::Command() {
}


Command::~Command() {
}

sf::Packet& Command::packetIn(sf::Packet& packet) {
	return packet; //DO NOT READ ID IN, it has already been read in
}

sf::Packet& Command::packetOut(sf::Packet& packet) const{
	return packet << id; //Send ID so class can be identified
}

sf::Packet& operator<<(sf::Packet& packet, const Command& command) {
	return command.packetOut(packet);
}

sf::Packet& operator>>(sf::Packet& packet, Command& command) {
	return command.packetIn(packet);
}
