#pragma once
#include "EntityCore.h"

class Ship:
	public EntityCore {
public:
	Ship();
	~Ship();
	void moveForward(float vel);
	void moveRight(float vel);
	void rotate(float ang);
	virtual void update(double dt);
protected:
	virtual void packetIn(sf::Packet& packet);
	virtual void packetOut(sf::Packet& packet);
	//The thrust being applied by the engines
	float forwardThrust = 0.0f;
	float sidewaysThrust = 0.0f;
	static const float decel;
};

