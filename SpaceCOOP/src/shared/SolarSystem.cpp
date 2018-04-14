#include "SolarSystem.h"
#include "../shared/Helper.h"


SolarSystem::SolarSystem() {
}


SolarSystem::~SolarSystem() {
}

void SolarSystem::addPlanet(shared_ptr<Planet> planet) {
	planets.push_back(planet);
}

void SolarSystem::addStar(shared_ptr<EntityStar> star) {
	this->star = star;
}
