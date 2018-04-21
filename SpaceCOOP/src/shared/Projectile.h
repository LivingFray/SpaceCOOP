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

	friend sf::Packet& operator<<(sf::Packet& packet, const Projectile& command);
	friend sf::Packet& operator >> (sf::Packet& packet, Projectile& command);

	SolarSystem* system;

	UUID origin;
	sf::Vector2f direction;
	sf::Uint8 type;
	//Age of projectile
	float TTL = 1.0f;
protected:
	virtual void packetIn(sf::Packet& packet);
	virtual void packetOut(sf::Packet& packet) const;
};

