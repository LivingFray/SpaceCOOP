#pragma once
#include <vector>
#include <memory>
#include <random>
#include <unordered_map>
#include "entities/Planet.h"
#include "entities/EntityStar.h"
#include "Projectile.h"

using std::vector;
using std::unordered_map;
using std::shared_ptr;

//TODO: Look into noise functions for reproducable seed based galaxies

class SolarSystem {
public:
	SolarSystem();
	virtual ~SolarSystem();
	virtual void addPlanet(shared_ptr<Planet> planet);
	virtual void addStar(shared_ptr<EntityStar> star);
	virtual void addEntity(shared_ptr<EntityCore> entity) = 0;
	virtual shared_ptr<EntityCore> getEntity(UUID id);
	virtual void removeEntity(UUID id) = 0;
	virtual void removeAll();

	virtual void addProjectile(shared_ptr<Projectile> proj);
protected:
	vector<shared_ptr<Planet>> planets;
	unordered_map<UUID, shared_ptr<EntityCore>> entities;
	vector<shared_ptr<Projectile>> projectiles;
	shared_ptr<EntityStar> star;
};

