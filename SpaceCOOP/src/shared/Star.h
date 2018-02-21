#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>

class Star {
public:
	Star();
	virtual ~Star();
	sf::Vector2f position;
	sf::Color colour;
	float radius;

	//TODO: Add more featues such as planets

	friend sf::Packet& operator <<(sf::Packet& packet, const Star& star);
	friend sf::Packet& operator >>(sf::Packet& packet, Star& star);
};

