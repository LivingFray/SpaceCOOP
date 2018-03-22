#include "EntityCore.h"
#include "../AssetHandler.h"
#include <chrono>
#include "../Helper.h"


EntityCore::EntityCore() {
	//Set the unique id to be the current time, because its the easiest way to ensure uniqueness
	id = static_cast<UUID>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

EntityCore::~EntityCore() {
}

void EntityCore::setPosition(const sf::Vector2f& p) {
	auto current = getPosition();
	posChanged = current != p;
	Transformable::setPosition(p);
}

void EntityCore::setRotation(float angle) {
	auto current = getRotation();
	angleChanged = current != angle;
	Transformable::setRotation(angle);
}

void EntityCore::setVelocity(sf::Vector2f v) {
	auto current = getVelocity();
	velChanged = current != v;
	vel = v;
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
	//Go through each tracked property and add it to the packet if it needs updating
	//Reset relevant changed flags as states are now synced
	//TODO: Only sync if it is likely the client has desynced, to save bandwidth
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
	if (angleChanged) {
		p << ANG_MODID;
		p << getRotation();
		angleChanged = false;
	}
	if (angMomentumChanged) {
		p << ANGMOM_MODID;
		p << angMomentum;
		angMomentumChanged = false;
	}
}

void EntityCore::update(double dt) {
	//TODO: More useful update things
	sf::Vector2f newPos = getPosition();
	newPos += getVelocity() * static_cast<float>(dt);
	setPosition(newPos);
	if (abs(angMomentum) > 1e-5) {
		float newAng = getRotation();
		newAng += angMomentum * static_cast<float>(dt);
		setRotation(newAng);
	}
}

sf::Vector2f EntityCore::getFront() {
	float ang = -toDegrees(getRotation());
	sf::Vector2f dir(cosf(ang), -sinf(ang));
	return dir;
	//return dir / sqrtf(dir.x * dir.x + dir.y * dir.y);
}

sf::Vector2f EntityCore::getRight() {
	float ang = -toDegrees(getRotation());
	sf::Vector2f dir(-cosf(ang), -sinf(ang));
	return dir;
}

void EntityCore::packetIn(sf::Packet& packet) {
	//DO NOT READ IN UUID, this is already read in to determine which entity to call packetIn from
	sf::Vector2f pos;
	packet >> pos.x >> pos.y;
	Transformable::setPosition(pos);
	packet >> vel.x >> vel.y;
	float ang;
	packet >> ang;
	Transformable::setRotation(ang);
	packet >> angMomentum;
	//TODO: Other core data
}

void EntityCore::packetOut(sf::Packet& packet) const {
	packet << id;
	sf::Vector2f pos = Transformable::getPosition();
	packet << pos.x << pos.y;
	packet << vel.x << vel.y;
	packet << getRotation();
	packet << angMomentum;
}

bool EntityCore::applyModification(sf::Uint8 modId, sf::Packet& p) {
	if (modId >= NUM_MODIFY_PROTOCOLS) {
		return false;
	}
	switch (modId) {
		case POS_MODID:
		{
			//Position change
			sf::Vector2f pos;
			p >> pos.x;
			p >> pos.y;
			Transformable::setPosition(pos);
			return true;
		}
		case VEL_MODID:
		{
			//Velocity change
			p >> vel.x;
			p >> vel.y;
			return true;
		}
		case ANG_MODID:
		{
			//Angle change
			float ang;
			p >> ang;
			Transformable::setRotation(ang);
			return true;
		}
		case ANGMOM_MODID:
		{
			//Angular Momentum change
			p >> angMomentum;
			return true;
		}
	}
	return false;
}

void EntityCore::loadSpriteAndResize(std::string texture) {
	sprite.setTexture(AssetHandler::getTexture(texture));
	auto bounds = sprite.getLocalBounds();
	float sX = width / bounds.width;
	float sY = height / bounds.height;
	setScale(sX, sY);
	setOrigin(bounds.width * 0.5, bounds.height * 0.5);
}

void EntityCore::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(sprite, states);
}

sf::Packet& operator<<(sf::Packet& packet, const EntityCore& command) {
	command.packetOut(packet);
	return packet;
}

sf::Packet & operator >> (sf::Packet& packet, EntityCore& command) {
	command.packetIn(packet);
	return packet;
}
