#include "Ship.h"
#include "../AssetHandler.h"
#include <algorithm>

const float Ship::decel = 5.0f;

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
	if (abs(forwardThrust) < 1e-5 && abs(sidewaysThrust) < 1e-5) {
		//Decelerate if not trying to move
		//Get unit direction
		sf::Vector2f dir = getVelocity();
		float speed = sqrtf(dir.x * dir.x + dir.y * dir.y);
		if (speed > 0.0f) {
			dir /= speed;
			float newSpeed = std::max(0.0f, static_cast<float>(speed - decel * dt));
			setVelocity(dir * newSpeed);
		}
	} else {
		sf::Vector2f thrust = getFront() * forwardThrust;
		thrust += getRight() * sidewaysThrust;
		thrust *= static_cast<float>(dt);
		setVelocity(getVelocity() + thrust);
	}
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
