#pragma once
#include <SFML\Graphics.hpp>
#include "..\shared\Galaxy.h"
class ClientGalaxy :
	public Galaxy,
	public sf::Drawable {
public:
	ClientGalaxy();
	~ClientGalaxy();

	void createVertexArray();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::VertexArray vertexArray;
	sf::Shader starShader;
};

