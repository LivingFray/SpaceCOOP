#include "HUD.h"
#include "Client.h"
#include <string>

HUD::HUD() {
	font.loadFromFile("assets/cour.ttf");
	health.setFont(font);
	health.setCharacterSize(24);
	health.setPosition(sf::Vector2f(10.0f, 10.0f));
	health.setFillColor(sf::Color(255, 255, 255));
}


HUD::~HUD() {
}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.setView(HUDView);
	target.draw(health, states);
}


void HUD::resize(unsigned int width, unsigned int height) {
	HUDView = sf::View(sf::FloatRect(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)));
}

void HUD::update() {
	if (client->getShip()) {
		std::string str = "Health: " + std::to_string(client->getShip()->getHealth());
		health.setString(str);
	} else {
		health.setString("Health: 0");
	}
}
