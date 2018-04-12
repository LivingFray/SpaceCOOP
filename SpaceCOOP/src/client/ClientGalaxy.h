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
	//Selects a star close to the coordinates (in local space) given (keeps last if no near stars)
	void selectStar(float x, float y);

	int selected = 0;

	//Bounding box of vertex array
	float minX, minY, maxX, maxY;
private:
	sf::VertexArray vertexArray;
	sf::Shader starShader;
	sf::CircleShape outline;
};

