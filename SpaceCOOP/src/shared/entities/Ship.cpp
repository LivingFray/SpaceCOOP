#include "Ship.h"
#include "../AssetHandler.h"

Ship::Ship() {
	type = 0;
	width = 100;
	height = 50;
	sprite.setTexture(AssetHandler::getTexture("assets/ship.png"));
	auto bounds = sprite.getLocalBounds();
	float sX = width / bounds.width;
	float sY = height / bounds.height;
	setScale(sX, sY);
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
