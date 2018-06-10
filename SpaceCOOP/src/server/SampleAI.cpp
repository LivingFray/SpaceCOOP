#include "SampleAI.h"
#include "../shared/Helper.h"
#include "ServerSolarSystem.h"


SampleAI::SampleAI(ServerSolarSystem* solarSystem, shared_ptr<Ship> controls) : AI(solarSystem, controls) {
	currentTarget = sf::Vector2f(randF(-500.0f, 500.0f), randF(-500.0f, 500.0f));
	ship->setDesiredAngle(toDegrees(atan2f(currentTarget.y - ship->getPosition().y, currentTarget.x - ship->getPosition().x)));
	atTarget = false;
}

SampleAI::~SampleAI() {
}

void SampleAI::update(double dt) {
	if (!atTarget) {
		if (ship->atDesiredAngle()) {
			float d = magnitude(ship->getPosition() - currentTarget);
			if (d > 100.0f || (magnitude(ship->getVelocity()) < 10.0f && d > 10.0f)) {
				ship->moveForward(0.1f);
			} else {
				//Slow down
				ship->moveForward(0.0f);
				//Point at target
				if (d < 50.0f) {
					atTarget = true;
					sf::Vector2f target = solarSystem->getPlayers()[0]->ship->getPosition();
					ship->setDesiredAngle(toDegrees(atan2f(target.y - ship->getPosition().y, target.x - ship->getPosition().x)));
				}
			}
		}
	} else {
		if (ship->atDesiredAngle()) {
			//Fire weapon
			ship->fire();
			//Setup next target
			currentTarget = sf::Vector2f(randF(-500.0f, 500.0f), randF(-500.0f, 500.0f));
			ship->setDesiredAngle(toDegrees(atan2f(currentTarget.y - ship->getPosition().y, currentTarget.x - ship->getPosition().x)));
			atTarget = false;
		}
	}
}
