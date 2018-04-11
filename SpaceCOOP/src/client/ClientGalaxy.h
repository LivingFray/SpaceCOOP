#pragma once
#include <SFML\Graphics.hpp>
#include "..\shared\Galaxy.h"
class ClientGalaxy :
	public Galaxy,
	public sf::Drawable,
	public sf::Transformable {
public:
	ClientGalaxy();
	~ClientGalaxy();

	void createVertexArray();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//Bounding box of vertex array
	float minX, minY, maxX, maxY;
private:
	sf::VertexArray vertexArray;
	sf::Shader starShader;
};

