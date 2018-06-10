#include "AI.h"
#include "Server.h"



AI::AI(ServerSolarSystem* solarSystem, shared_ptr<Ship> controls) {
	this->solarSystem = solarSystem;
	ship = controls;
	console = solarSystem->getConsole();
}

AI::~AI() {
}

void AI::update(double dt) {
}
