#pragma once
#include "State.h"
#include <SFML/Graphics.hpp>
class TitleScreen :
	public State {
public:
	TitleScreen();
	~TitleScreen();

	void update(double dt);
	void draw();
	void keyEvent(sf::Event e);
	void textEvent(sf::Event e);
	void resizeEvent(sf::Event e);

	bool switched = false;
private:
	sf::Font font;
};

