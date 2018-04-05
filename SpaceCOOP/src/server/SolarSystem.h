#pragma once
#include <vector>
#include <memory>
#include <random>

#include "../shared/entities/Planet.h"

class Server;

using std::vector;
using std::shared_ptr;

//TODO: Look into noise functions for reproducable seed based galaxies

class SolarSystem {
public:
	SolarSystem();
	~SolarSystem();

	//Pass in info about star?
	void generateSystem(Server* server);

	std::default_random_engine generator;

protected:
	vector<shared_ptr<Planet>> planets;
	//Star object here
};

