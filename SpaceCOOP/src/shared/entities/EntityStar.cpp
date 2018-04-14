#include "EntityStar.h"



EntityStar::EntityStar() : EntityStar(1.0f) {
}


EntityStar::EntityStar(float radius) : EntityCore() {
	width = radius * 2;
	height = radius * 2;
	useShader = true;
	shader.loadFromFile("shaders/colourBurn.frag", sf::Shader::Fragment);
	shader.setUniform("tex", sf::Shader::CurrentTexture);
	loadSpriteAndResize("assets/star.png");
	type = 2;
}

EntityStar::~EntityStar() {
}

void EntityStar::setColour(sf::Color colour) {
	sprite.setColor(colour);
	this->colour = colour;
}

void EntityStar::packetIn(sf::Packet& packet) {
	EntityCore::packetIn(packet);
	sf::Color cIn;
	packet >> cIn.r;
	packet >> cIn.g;
	packet >> cIn.b;
	packet >> cIn.a;
	setColour(cIn);
}

void EntityStar::packetOut(sf::Packet& packet) const {
	EntityCore::packetOut(packet);
	packet << colour.r;
	packet << colour.g;
	packet << colour.b;
	packet << colour.a;
}
