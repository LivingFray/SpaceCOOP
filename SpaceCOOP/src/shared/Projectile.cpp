#include "Projectile.h"



Projectile::Projectile() {
}


Projectile::~Projectile() {
}

void Projectile::update(double dt) {
	if (!hasFired) {
		hasFired = true;
		onCreation();
	}
	TTL -= dt;
}

void Projectile::packetIn(sf::Packet& packet) {
	//Type should have already been read in at this point
	packet >> origin;
	packet >> direction.x;
	packet >> direction.y;
}

void Projectile::packetOut(sf::Packet& packet) const {
	packet << type;
	packet << origin;
	packet << direction.x;
	packet << direction.y;
}


sf::Packet& operator<<(sf::Packet& packet, const Projectile& projectile) {
	projectile.packetOut(packet);
	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Projectile& projectile) {
	projectile.packetIn(packet);
	return packet;
}
