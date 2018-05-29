#pragma once
#include <vector>
#include <memory>
#include <random>
#include <unordered_map>
#include <mutex>
#include "entities/Planet.h"
#include "entities/EntityStar.h"
#include "Projectile.h"

using std::vector;
using std::unordered_map;
using std::shared_ptr;
using std::mutex;

//TODO: Look into noise functions for reproducable seed based galaxies

class SolarSystem {
public:
	SolarSystem();
	virtual ~SolarSystem();
	virtual void addPlanet(shared_ptr<Planet> planet);
	virtual void addStar(shared_ptr<EntityStar> star);
	virtual void addEntity(shared_ptr<EntityCore> entity);
	virtual shared_ptr<EntityCore> getEntity(UUID id);

	virtual unordered_map<UUID, shared_ptr<EntityCore>> getEntities();
	//TODO: Add other getters as needed
	///CHange remove to be id, inc vectors
	virtual void removeEntity(UUID id);
	virtual void removeAll();

	virtual void addProjectile(shared_ptr<Projectile> proj);

	virtual void update(double dt);
protected:
	unordered_map<UUID, shared_ptr<Planet>> planets;
	unordered_map<UUID, shared_ptr<EntityCore>> entities;
	vector<shared_ptr<Projectile>> projectiles;
	shared_ptr<EntityStar> star;

	vector<shared_ptr<Planet>> toAddPlanets;
	vector<shared_ptr<EntityCore>> toAddEntities;
	vector<shared_ptr<Projectile>> toAddProjectiles;
	//TODO: Binary systems / multiple stars
	vector<shared_ptr<EntityStar>> toAddStars;

	vector<shared_ptr<Planet>> toRemovePlanets;
	vector<UUID> toRemoveEntities;
	vector<shared_ptr<Projectile>> toRemoveProjectiles;
	//TODO: Binary systems / multiple stars
	vector<shared_ptr<EntityStar>> toRemoveStars;

	//Safely adds the objects to be added (must only ever be called in one thread)
	void updateToAdds();
	//Safely removes the objects to be removed (must only ever be called in same thread as toAdd)
	void updateToRemoves();
	//Needed for the get*() methods, as they can be called from anywhere
	mutable mutex entityMutex, planetMutex, projectileMutex, starMutex;
};

