#include "Ship.h"
#include "../AssetHandler.h"

Ship::Ship() {
	width = 100;
	height = 50;
	loadSpriteAndResize("assets/ship.png");
	type = 0;
	setRotation(30.0f);
}


Ship::~Ship() {
}

void Ship::moveForward(float vel) {
	forwardThrust = vel;
}

void Ship::moveRight(float vel) {
	sidewaysThrust = vel;
}

void Ship::rotate(float ang) {
	angMomentum = ang;
	angMomentumChanged = true;
}

void Ship::update(double dt) {
	sf::Vector2f thrust = getFront() * forwardThrust;
	thrust += getRight() * sidewaysThrust;
	thrust *= static_cast<float>(dt);
	setVelocity(getVelocity() + thrust);
	EntityCore::update(dt);
}

void Ship::packetIn(sf::Packet& packet) {
	EntityCore::packetIn(packet);
	//Ship specific stuff here
}

void Ship::packetOut(sf::Packet& packet) {
	EntityCore::packetOut(packet);
	//Ship specific stuff here
}
