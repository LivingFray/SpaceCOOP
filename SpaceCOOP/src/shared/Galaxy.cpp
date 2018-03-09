#include "Galaxy.h"



Galaxy::Galaxy() {
}


Galaxy::~Galaxy() {
}


sf::Packet& operator <<(sf::Packet& packet, const Galaxy& galaxy) {
	for (shared_ptr<Star> star : galaxy.stars) {
		packet = packet << *star;
	}
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Galaxy& galaxy) {
	while (!packet.endOfPacket()) {
		shared_ptr<Star> star(new Star());
		packet = packet >> *star;
		galaxy.stars.push_back(star);
	}
	return packet;
}