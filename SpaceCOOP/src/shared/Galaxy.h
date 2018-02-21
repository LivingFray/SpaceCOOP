#pragma once
#include <Vector>
#include "Star.h"
#include <memory>

class Galaxy {
public:
	Galaxy();
	~Galaxy();

	//TODO: Add more things like destination, black holes, nebula, etc
	std::vector<std::shared_ptr<Star>> stars;

	friend sf::Packet& operator <<(sf::Packet& packet, const Galaxy& galaxy);
	friend sf::Packet& operator >>(sf::Packet& packet, Galaxy& galaxy);
};

