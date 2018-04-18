#pragma once
#include "entities/EntityCore.h"
class Projectile {
public:
	Projectile();
	virtual ~Projectile();

	friend sf::Packet& operator<<(sf::Packet& packet, const Projectile& command);
	friend sf::Packet& operator>>(sf::Packet& packet, Projectile& command);

	UUID origin;
	sf::Vector2f direction;
	sf::Uint8 type;
protected:
	virtual void packetIn(sf::Packet& packet);
	virtual void packetOut(sf::Packet& packet) const;
};

