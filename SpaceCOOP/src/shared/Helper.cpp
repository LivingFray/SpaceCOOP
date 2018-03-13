#include "Helper.h"

void concatPackets(sf::Packet& primary, sf::Packet& extra) {
	sf::Uint8 byte;
	while (extra >> byte) {
		primary << byte;
	}
}