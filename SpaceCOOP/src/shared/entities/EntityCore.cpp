#include "EntityCore.h"
#include <chrono>



EntityCore::EntityCore() {
	//Set the unique id to be the current time, because its the easiest way to ensure uniqueness
	id = static_cast<UUID>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	type = -1;
}


EntityCore::~EntityCore() {
}

void EntityCore::setPos(sf::Vector2f p) {
	pos = p;
	posChanged = true;
}

void EntityCore::setVel(sf::Vector2f v) {
	vel = v;
	velChanged = true;
}

void EntityCore::modify(sf::Packet& p) {
	while (!p.endOfPacket()) {
		sf::Uint8 modId;
		p >> modId;
		applyModification(modId, p);
		if (!p) {
			//Throw a hissy fit here
		}
	}
}

//Called every time the server wants to update client states
void EntityCore::generateModifyPacket(sf::Packet& p) {
	if (posChanged) {
		p << POS_MODID;
		p << pos.x << pos.y;
		posChanged = false;
	}
	if (velChanged) {
		p << VEL_MODID;
		p << vel.x << vel.y;
		velChanged = false;
	}
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

bool EntityCore::applyModification(sf::Uint8 modId, sf::Packet& p) {
	if (modId >= NUM_MODIFY_PROTOCOLS) {
		return false;
	}
	switch (modId) {
	case POS_MODID: { //Position change
		p >> pos.x;
		p >> pos.y;
		return true;
	}
	case VEL_MODID: { //Velocity change
		p >> vel.x;
		p >> vel.y;
		return true;
	}
	}
	return false;
}

sf::Packet& operator<<(sf::Packet& packet, const EntityCore& command) {
	command.packetOut(packet);
	return packet;
}

sf::Packet & operator>>(sf::Packet& packet, EntityCore& command) {
	command.packetIn(packet);
	return packet;
}
