#pragma once
#include "..\shared\Galaxy.h"
class ServerGalaxy :
	public Galaxy {
public:
	ServerGalaxy();
	~ServerGalaxy();

	//TODO: Save/Load
	void generateGalaxy();
};

