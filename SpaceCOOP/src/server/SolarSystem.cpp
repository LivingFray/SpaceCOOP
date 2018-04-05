#include "SolarSystem.h"
#include "../shared/Helper.h"
#include "Server.h"


SolarSystem::SolarSystem() {
}


SolarSystem::~SolarSystem() {
}


void SolarSystem::generateSystem(Server* server) {
	//Random number of planets from 1 to 12
	std::uniform_int_distribution<int> intDist(1, 12);
	//Random position of planets in orbit
	std::uniform_real_distribution<float> angDist(0.0f, toRadians(360.0f));
	//Random altitude of orbits
	std::uniform_real_distribution<float> orbitDist(5000.0f, 30000.0f);
	//Random radius of planet
	std::uniform_real_distribution<float> radiusDist(50.0f, 500.0f);
	float previousOrbit = 0.0f; // Distance (in AU) the previous planet was from the star
	for (int i = 0; i < intDist(generator); i++) {
		//Move out and put planet at random rotation around star
		float orbit = previousOrbit + orbitDist(generator);
		previousOrbit = orbit;
		float angle = angDist(generator);
		sf::Vector2f planetPosition(cosf(angle), sinf(angle));
		planetPosition *= orbit;
		//Create new planet
		shared_ptr<Planet> planet = std::make_shared<Planet>(radiusDist(generator));
		planet->setPosition(planetPosition);
		planets.push_back(planet);
		//Register planet in server entity list (May wish to change later, or add reference to system in planet to allow for multiple systems loaded at once)
		server->addEntity(planet);
	}
}
