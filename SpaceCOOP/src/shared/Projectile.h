#pragma once
#include "entities/EntityCore.h"

class SolarSystem;

class Projectile :
	public sf::Transformable,
	public sf::Drawable {
public:
	Projectile();
	virtual ~Projectile();

	virtual void update(double dt);

	virtual void onCreation() {};

	void setSolarSystem(SolarSystem* sys);

	friend sf::Packet& operator<<(sf::Packet& packet, const Projectile& command);
	friend sf::Packet& operator >> (sf::Packet& packet, Projectile& command);


	UUID origin;
	sf::Vector2f direction;
	sf::Uint8 type;
	//Age of projectile
	float TTL = 1.0f;

	float damage;
protected:
	virtual void packetIn(sf::Packet& packet);
	virtual void packetOut(sf::Packet& packet) const;
	bool hasFired = false;
	SolarSystem* system;
	bool isServerSide;
};

