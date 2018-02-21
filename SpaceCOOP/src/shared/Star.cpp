#include "Star.h"



Star::Star() {
}


Star::~Star() {
}


sf::Packet& operator <<(sf::Packet& packet, const Star& star) {
	return packet << star.position.x << star.position.y << star.colour.r \
		<< star.colour.g << star.colour.b << star.colour.a << star.radius;
}

sf::Packet& operator >>(sf::Packet& packet, Star& star) {
	return packet >> star.position.x >> star.position.y >> star.colour.r \
		>> star.colour.g >> star.colour.b >> star.colour.a >> star.radius;
}