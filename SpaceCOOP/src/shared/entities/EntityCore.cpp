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
	sf::Uint8 packets[8];
	int i = 0;
	while (!p.endOfPacket()) {
		sf::Uint8 modId;
		p >> modId;
		packets[i] = modId;
		i++;
		applyModification(modId, p);
		if (!p) {
			//Throw a hissy fit here
			break;
		}
	}
}

//Called every time the server wants to update client states
void EntityCore::generateModifyPacket(sf::Packet& p) {
	//Go through each tracked property and add it to the packet if it needs updating
	//Reset relevant changed flags as states are now synced
	//TODO: Only sync if it is likely the client has desynced, to save bandwidth
	if (posChanged) {
		p << static_cast<sf::Uint8>(MODS::POS);
		sf::Vector2f pos = Transformable::getPosition();
		p << pos.x << pos.y;
		posChanged = false;
	}
	if (velChanged) {
		p << static_cast<sf::Uint8>(MODS::VEL);
		p << vel.x << vel.y;
		velChanged = false;
	}
	if (angleChanged) {
		p << static_cast<sf::Uint8>(MODS::ANG);
		p << getRotation();
		angleChanged = false;
	}
	if (angMomentumChanged) {
		p << static_cast<sf::Uint8>(MODS::ANG_MOM);
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
	float ang = -toRadians(getRotation());
	sf::Vector2f dir(cosf(ang), -sinf(ang));
	return dir;
	//return dir / sqrtf(dir.x * dir.x + dir.y * dir.y);
}

sf::Vector2f EntityCore::getRight() {
	float ang = -toRadians(getRotation() + 90.0f);
	sf::Vector2f dir(cosf(ang), -sinf(ang));
	return dir;
}

bool EntityCore::collidesWithLine(sf::Vector2f origin, sf::Vector2f direction) {
	//Transform line into local space
	origin = getInverseTransform().transformPoint(origin);
	direction = normalise(getInverseTransform().transformPoint(origin));
	//Lifted from the internet (used previously in my ray tracer)
	float top = sprite.getLocalBounds().top;
	float left = sprite.getLocalBounds().left;
	float bottom = top + sprite.getLocalBounds().height;
	float right = left + sprite.getLocalBounds().width;

	sf::Vector2f inv_d = normalise(sf::Vector2f(1.0f / direction.x, 1.0f / direction.y));

	float tx1 = (left - origin.x)*inv_d.x;
	float tx2 = (right - origin.x)*inv_d.x;

	float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);

	float ty1 = (top - origin.y)*inv_d.y;
	float ty2 = (bottom - origin.y)*inv_d.y;

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	return tmax >= tmin && tmax > 0.0f;
}


void EntityCore::packetIn(sf::Packet& packet) {
	//DO NOT READ IN UUID, this is already read in to determine which entity to call packetIn from
	//SAME FOR ENTITY TYPE
	sf::Vector2f pos;
	packet >> pos.x >> pos.y;
	Transformable::setPosition(pos);
	packet >> vel.x >> vel.y;
	float ang;
	packet >> ang;
	Transformable::setRotation(ang);
	packet >> angMomentum;
	packet >> width;
	packet >> height;
	resizeSprite();
}

void EntityCore::packetOut(sf::Packet& packet) const {
	packet << id;
	packet << type;
	sf::Vector2f pos = Transformable::getPosition();
	packet << pos.x << pos.y;
	packet << vel.x << vel.y;
	packet << getRotation();
	packet << angMomentum;
	packet << width;
	packet << height;
}

bool EntityCore::applyModification(sf::Uint8 modId, sf::Packet& p) {
	if (modId >= static_cast<sf::Uint8>(MODS::NUM_MODS)) {
		return false;
	}
	switch (modId) {
		case static_cast<sf::Uint8>(MODS::POS) :
		{
			//Position change
			sf::Vector2f pos;
			p >> pos.x;
			p >> pos.y;
			Transformable::setPosition(pos);
			return true;
		}
		case static_cast<sf::Uint8>(MODS::VEL) :
		{
			//Velocity change
			p >> vel.x;
			p >> vel.y;
			return true;
		}
		case static_cast<sf::Uint8>(MODS::ANG) :
		{
			//Angle change
			float ang;
			p >> ang;
			Transformable::setRotation(ang);
			return true;
		}
		case static_cast<sf::Uint8>(MODS::ANG_MOM) :
		{
			//Angular Momentum change
			p >> angMomentum;
			return true;
		}
	}
	return false;
}

void EntityCore::loadSpriteAndResize(std::string texture) {
	//Textures load in at the size of the image, this isn't too helpful
	sprite.setTexture(AssetHandler::getTexture(texture));
	resizeSprite();
}

void EntityCore::resizeSprite() {
	auto bounds = sprite.getLocalBounds();
	float sX = width / bounds.width;
	float sY = height / bounds.height;
	setScale(sX, sY);
	setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
}

void EntityCore::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (useShader) {
		states.shader = &shader;
	}
	states.transform *= getTransform();
	target.draw(sprite, states);
}

sf::Packet& operator<<(sf::Packet& packet, const EntityCore& entity) {
	entity.packetOut(packet);
	return packet;
}

sf::Packet & operator >> (sf::Packet& packet, EntityCore& entity) {
	entity.packetIn(packet);
	return packet;
}
