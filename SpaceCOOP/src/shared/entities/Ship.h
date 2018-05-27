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

	float getTurnRate() const { return maxTurnRate; }

	void setDesiredAngle(float angle);

	void fire();

	void setHealth(float health);
	float getHealth();

	void setMaxHealth(float maxHealth);
	float getMaxHealth();

protected:
	virtual void packetIn(sf::Packet& packet);
	virtual void packetOut(sf::Packet& packet);
	virtual bool applyModification(sf::Uint8, sf::Packet& p);
	virtual void generateModifyPacket(sf::Packet& p);
	//The thrust being applied by the engines
	float forwardThrust = 0.0f;
	float sidewaysThrust = 0.0f;
	//The effective thrust of the ship when no key is pressed
	float decel = 100.0f;
	//The maximum speed the ship can accelerate by
	float acceleration = 50.0f;
	//The maximum rate the ship can turn
	float maxTurnRate = 720.0f;
	//The angle the ship is trying to rotate to
	float desiredAngle;
	bool rotateToDesired = false;

	float maxHealth;
	float health;
	bool healthChanged = false;
	bool maxHealthChanged = false;

	enum class MODS { HEALTH, MAX_HEALTH, NUM_MODS };
};

