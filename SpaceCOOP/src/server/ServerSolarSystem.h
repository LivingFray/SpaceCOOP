#pragma once
#include "../shared/SolarSystem.h"

class Server;

class ServerSolarSystem :
	public SolarSystem {
public:
	ServerSolarSystem();
	~ServerSolarSystem();
	//Pass in info about star?
	void generateSystem(Server* server);

	std::default_random_engine generator;
};

