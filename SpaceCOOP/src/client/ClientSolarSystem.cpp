#include "ClientSolarSystem.h"
#include "Client.h"


ClientSolarSystem::ClientSolarSystem() {
	//starShader.loadFromFile("shaders/bigstar.frag", sf::Shader::Fragment);
}


ClientSolarSystem::~ClientSolarSystem() {
}

void ClientSolarSystem::init() {
	window = client->window;
}

void ClientSolarSystem::addPlanet(shared_ptr<Planet> planet) {
	SolarSystem::addPlanet(planet);
	adjustView();
}

void ClientSolarSystem::draw() const {
	if (mapVisible) {
		window->setView(view);
		sf::RectangleShape backing;
		backing.setPosition(-maxSize / 2.0f, -maxSize / 2.0f);
		backing.setSize(sf::Vector2f(maxSize, maxSize));
		backing.setFillColor(sf::Color(0, 0, 53, 127));
		window->draw(backing);
		for (auto planet : planets) {
			sf::Vector2f scale = planet->getScale();
			planet->setScale(scale * 4.0f);
			window->draw(*planet);
			planet->setScale(scale);
		}
		if(star) {
			sf::Vector2f scale = star->getScale();
			star->setScale(scale * 4.0f);
			window->draw(*star);
			star->setScale(scale);
		}
		auto ship = client->getShip();
		if (ship) {
			sf::Vector2f scale = ship->getScale();
			ship->setScale(scale * 40.0f);
			window->draw(*ship);
			ship->setScale(scale);
		}
	}
}

void ClientSolarSystem::resize(unsigned int width, unsigned int height) {
	sWidth = width;
	sHeight = height;
	adjustView();
}

void ClientSolarSystem::reset() {
	planets.clear();
	adjustView();
}

void ClientSolarSystem::adjustView() {
	maxSize = 1;
	//Calculate smallest square that covers entire system
	for (auto planet : planets) {
		sf::Vector2f pos = planet->getPosition();
		if (abs(pos.x) > maxSize) {
			maxSize = abs(pos.x);
		}
		if (abs(pos.y) > maxSize) {
			maxSize = abs(pos.y);
		}
	}
	//Fudge factor to give a bit of a border between furthest out and edge of map
	//Also doubles as set size takes total screen size, not size from centre
	maxSize *= 2.2f;
	float smallest = static_cast<float>(sWidth < sHeight ? sWidth : sHeight);
	view.setSize(maxSize * (sWidth / smallest), maxSize * (sHeight / smallest));
}
