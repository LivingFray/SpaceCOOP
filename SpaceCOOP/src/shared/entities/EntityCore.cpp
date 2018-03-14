#include "EntityCore.h"
#include <chrono>



EntityCore::EntityCore() {
	//Set the unique id to be the current time, because its the easiest way to ensure uniqueness
	id = static_cast<UUID>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	type = -1;
}


EntityCore::~EntityCore() {
}

void EntityCore::setPosition(const sf::Vector2f& p) {
	posChanged = true;
	Transformable::setPosition(p);
}

void EntityCore::setVelocity(sf::Vector2f v) {
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
		sf::Vector2f pos = Transformable::getPosition();
		p << pos.x << pos.y;
		posChanged = false;
	}
	if (velChanged) {
		p << VEL_MODID;
		p << vel.x << vel.y;
		velChanged = false;
	}
}

void EntityCore::update(double dt) {
	//TODO: More useful update things
	sf::Vector2f newPos = getPosition();
	newPos += getVelocity() * static_cast<float>(dt);
	setPosition(newPos);
}

void EntityCore::packetIn(sf::Packet& packet) {
	//DO NOT READ IN UUID, this is already read in to determine which entity to call packetIn from
	sf::Vector2f pos;
	packet >> pos.x >> pos.y;
	Transformable::setPosition(pos);
	packet >> vel.x >> vel.y;
	//TODO: Other core data
}

void EntityCore::packetOut(sf::Packet& packet) const{
	packet << id;
	sf::Vector2f pos = Transformable::getPosition();
	packet << pos.x << pos.y;
	packet << vel.x << vel.y;
}

bool EntityCore::applyModification(sf::Uint8 modId, sf::Packet& p) {
	if (modId >= NUM_MODIFY_PROTOCOLS) {
		return false;
	}
	switch (modId) {
	case POS_MODID: { //Position change
		sf::Vector2f pos;
		p >> pos.x;
		p >> pos.y;
		Transformable::setPosition(pos);
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

void EntityCore::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(sprite, states);
}

sf::Packet& operator<<(sf::Packet& packet, const EntityCore& command) {
	command.packetOut(packet);
	return packet;
}

sf::Packet & operator>>(sf::Packet& packet, EntityCore& command) {
	command.packetIn(packet);
	return packet;
}
