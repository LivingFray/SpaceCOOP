#pragma once
#include "EntityCore.h"
class EntityStar :
	public EntityCore {
public:
	EntityStar();
	EntityStar(float radius);
	~EntityStar();

	void setColour(sf::Color colour);
protected:
	virtual void packetIn(sf::Packet& packet);
	virtual void packetOut(sf::Packet& packet) const;
private:
	sf::Color colour;
};

