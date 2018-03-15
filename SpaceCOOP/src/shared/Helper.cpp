#include "Helper.h"

void concatPackets(sf::Packet& primary, sf::Packet& extra) {
	sf::Uint8 byte;
	while (extra >> byte) {
		primary << byte;
	}
}

inline float toDegrees(float rad) {
	return 0.01745329251f * rad;
}
