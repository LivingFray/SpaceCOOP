#pragma once
#include "../shared/entities/Ship.h"
#include "../shared/Console.h"
#include <memory>

using std::shared_ptr;

class ServerSolarSystem;

class AI {
public:
	AI(ServerSolarSystem* solarSystem, shared_ptr<Ship> controls);

	virtual ~AI();

	virtual void update(double dt);

protected:
	ServerSolarSystem* solarSystem;
	shared_ptr<Console> console;
	shared_ptr<Ship> ship;
};

