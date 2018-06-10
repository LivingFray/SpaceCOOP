#pragma once
//Test AI that picks a random spot, goes to it then shoots at a player, and repeats
#include "AI.h"
class SampleAI :
	public AI {
public:
	SampleAI(ServerSolarSystem* solarSystem, shared_ptr<Ship> controls);
	virtual ~SampleAI();

	void update(double dt);
protected:
	sf::Vector2f currentTarget;
	bool atTarget;
};

