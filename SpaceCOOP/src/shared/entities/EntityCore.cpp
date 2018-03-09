#include "EntityCore.h"
#include <chrono>



EntityCore::EntityCore() {
	//Set the unique id to be the current time, because its the easiest way to ensure uniqueness
	id = static_cast<UUID>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	type = -1;
}


EntityCore::~EntityCore() {
}

void EntityCore::packetIn(sf::Packet& packet) {
	//DO NOT READ IN UUID, this is already read in to determine which entity to call packetIn from
	packet >> pos.x >> pos.y;
	packet >> vel.x >> vel.y;
	//TODO: Other core data
}

void EntityCore::packetOut(sf::Packet& packet) const{
	packet << id;
	packet << pos.x << pos.y;
	packet << vel.x << vel.y;
}

sf::Packet& operator<<(sf::Packet& packet, const EntityCore& command) {
	command.packetOut(packet);
	return packet;
}

sf::Packet & operator>>(sf::Packet& packet, EntityCore& command) {
	command.packetIn(packet);
	return packet;
}
