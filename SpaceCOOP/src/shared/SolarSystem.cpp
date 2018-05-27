#include "SolarSystem.h"
#include "../shared/Helper.h"


SolarSystem::SolarSystem() {
}


SolarSystem::~SolarSystem() {
}

void SolarSystem::addEntity(shared_ptr<EntityCore> ent) {
	toAddEntities.push_back(ent);
}

void SolarSystem::addPlanet(shared_ptr<Planet> planet) {
	toAddPlanets.push_back(planet);
}

void SolarSystem::addStar(shared_ptr<EntityStar> star) {
	toAddStars.push_back(star);
}

void SolarSystem::removeEntity(UUID entity) {
	toRemoveEntities.push_back(entity);
}

void SolarSystem::removeAll() {
	//TODO: Dethreadify
	entities.clear();
	projectiles.clear();
	planets.clear();
	star = NULL;
}

void SolarSystem::addProjectile(shared_ptr<Projectile> proj) {
	proj->system = this;
	toAddProjectiles.push_back(proj);
}

void SolarSystem::update(double dt) {
	//TODO: Move all duplicated code here
	updateToAdds();
	updateToRemoves();
}

void SolarSystem::updateToAdds() {
	//Add entities
	entityMutex.lock();
	for (auto ent : toAddEntities) {
		entities.insert_or_assign(ent->id, ent);
	}
	toAddEntities.clear();
	entityMutex.unlock();
	//Add planets
	planetMutex.lock();
	for (auto planet : toAddPlanets) {
		planets.insert_or_assign(planet->id, planet);
	}
	toAddPlanets.clear();
	planetMutex.unlock();
	//Add projectiles
	projectileMutex.lock();
	for (auto proj : toAddProjectiles) {
		projectiles.push_back(proj);
	}
	toAddProjectiles.clear();
	projectileMutex.unlock();
	//Add stars (WIP)
	starMutex.lock();
	for (auto s : toAddStars) {
		star = s;
	}
	toAddStars.clear();
	starMutex.unlock();
}

void SolarSystem::updateToRemoves() {
	//Remove entities
	entityMutex.lock();
	for (auto id : toRemoveEntities) {
		entities.erase(id);
	}
	toRemoveEntities.clear();
	entityMutex.unlock();
	//Remove planets
	planetMutex.lock();
	for (auto planet : toRemovePlanets) {
		planets.erase(planet->id);
	}
	toRemovePlanets.clear();
	planetMutex.unlock();
	//Remove projectiles
	projectileMutex.lock();
	for (auto proj : toRemoveProjectiles) {
		//This is a mess, O(n^2) removals != efficient
		//Change to map?
		for (auto pIt = projectiles.begin(); pIt != projectiles.end();) {
			if (*pIt == proj) {
				pIt = projectiles.erase(pIt);
			} else {
				pIt++;
			}
		}
	}
	toRemoveProjectiles.clear();
	projectileMutex.unlock();
	//Remove stars (WIP)
	starMutex.lock();
	for (auto s : toRemoveStars) {
		//star = s;
	}
	toRemoveStars.clear();
	starMutex.unlock();
}

shared_ptr<EntityCore> SolarSystem::getEntity(UUID id) {
	std::lock_guard<mutex> lock(entityMutex);
	if (entities.find(id) != entities.end()) {
		return entities[id];
	} else {
		return NULL;
	}
}