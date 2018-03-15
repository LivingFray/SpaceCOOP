#include "Ship.h"
#include "../AssetHandler.h"

Ship::Ship() {
	width = 100;
	height = 50;
	loadSpriteAndResize("assets/ship.png");
	type = 0;
}


Ship::~Ship() {
}

void Ship::packetIn(sf::Packet& packet) {
	EntityCore::packetIn(packet);
	//Ship specific stuff here
}

void Ship::packetOut(sf::Packet& packet) {
	EntityCore::packetOut(packet);
	//Ship specific stuff here
}
