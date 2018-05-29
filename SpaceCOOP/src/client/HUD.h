#pragma once
#include <SFML/Graphics.hpp>

class Client;

class HUD :
	public sf::Drawable {
public:
	HUD();
	virtual ~HUD();
	void resize(unsigned int width, unsigned int height);

	void update();

	Client* client;
protected:
	sf::View HUDView;
	sf::Text health;

	sf::Font font;
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

