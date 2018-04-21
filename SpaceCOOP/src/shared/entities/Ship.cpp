#include "Ship.h"
#include "../AssetHandler.h"
#include "../LaserBeam.h"
#include <algorithm>
#include <memory>
#include "../SolarSystem.h"
using std::shared_ptr;

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
	forwardThrust = acceleration * vel;
}

void Ship::moveRight(float vel) {
	sidewaysThrust = acceleration * vel;
}

void Ship::rotate(float ang) {
	angMomentum = std::max(-maxTurnRate, std::min(ang, maxTurnRate));
	angMomentumChanged = true;
}

void Ship::update(double dt) {
	if (rotateToDesired) {
		float rotateBy = desiredAngle - getRotation();
		if (rotateBy > 180.0f) {
			rotateBy -= 360.0f;
		} else if (rotateBy < -180.0f) {
			rotateBy += 360.0f;
		}
		float absRotate = abs(rotateBy);
		if (absRotate > maxTurnRate * dt) {
			rotateBy /= absRotate;
			rotateBy *= maxTurnRate;
		} else {
			rotateToDesired = false;
		}
		rotate(rotateBy);
	}
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

void Ship::setDesiredAngle(float angle) {
	desiredAngle = angle;
	rotateToDesired = true;
}

void Ship::fire() {
	//TODO: Weapon selection and such
	shared_ptr<LaserBeam> beam = std::make_shared<LaserBeam>();
	beam->origin = id;
	beam->direction = getFront();
	system->addProjectile(beam);
}

void Ship::packetIn(sf::Packet& packet) {
	EntityCore::packetIn(packet);
	//Ship specific stuff here
}

void Ship::packetOut(sf::Packet& packet) {
	EntityCore::packetOut(packet);
	//Ship specific stuff here
}
