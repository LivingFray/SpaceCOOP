#pragma once
#include "EntityCore.h"

class Ship:
	public EntityCore {
public:
	Ship();
	~Ship();
protected:
	virtual void packetIn(sf::Packet& packet);
	virtual void packetOut(sf::Packet& packet);
};

