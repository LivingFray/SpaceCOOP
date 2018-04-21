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

void SolarSystem::removeAll() {
	entities.clear();
	projectiles.clear();
	planets.clear();
	star = NULL;
}

void SolarSystem::addProjectile(shared_ptr<Projectile> proj) {
	proj->system = this;
	projectiles.push_back(proj);
}

shared_ptr<EntityCore> SolarSystem::getEntity(UUID id) {
	if (entities.find(id) != entities.end()) {
		return entities[id];
	} else {
		return NULL;
	}
}