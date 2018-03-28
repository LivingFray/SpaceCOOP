#pragma once
#include <SFML/Graphics.hpp>
class State {
public:
	State();
	virtual ~State();
	virtual void update(double dt) = 0;
	virtual void draw() = 0;
	virtual void keyEvent(sf::Event e) = 0;
	virtual void textEvent(sf::Event e) = 0;
	sf::RenderWindow* window;
};

