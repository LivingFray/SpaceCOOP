#pragma once
#include <SFML/Graphics.hpp>

class Client;

class Background:
	public sf::Drawable {
public:
	Background();
	~Background();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void generate();
	void setAspect(unsigned int width, unsigned int height);
	Client* client;
private:
	sf::VertexArray starVerts;
	sf::VertexArray starFarVerts;
	sf::Shader starShader;
	sf::View backgroundView;
};

