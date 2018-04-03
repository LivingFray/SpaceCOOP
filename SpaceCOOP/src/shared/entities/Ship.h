#pragma once
#include "EntityCore.h"

class Ship:
	public EntityCore {
public:
	Ship();
	~Ship();
	//Accelerates the ship by a proportion of the acceleration speed
	void moveForward(float vel);
	void moveRight(float vel);
	void rotate(float ang);
	virtual void update(double dt);
	//The amount to rotate the ship by when using keys
	float rotateAmount = 90.0f;
protected:
	virtual void packetIn(sf::Packet& packet);
	virtual void packetOut(sf::Packet& packet);
	//The thrust being applied by the engines
	float forwardThrust = 0.0f;
	float sidewaysThrust = 0.0f;
	static const float decel;
	//The maximum speed the ship can accelerate by
	float acceleration = 50.0f;
};

