#pragma once
#include <vector>
#include <memory>
#include <random>
#include "entities/Planet.h"
#include "entities/EntityStar.h"

using std::vector;
using std::shared_ptr;

//TODO: Look into noise functions for reproducable seed based galaxies

class SolarSystem {
public:
	SolarSystem();
	virtual ~SolarSystem();
	virtual void addPlanet(shared_ptr<Planet> planet);
	virtual void addStar(shared_ptr<EntityStar> star);
protected:
	vector<shared_ptr<Planet>> planets;
	//Star object here
	shared_ptr<EntityStar> star;
};

