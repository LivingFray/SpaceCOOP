#include "ClientGalaxy.h"
#include "../shared/Console.h"

using std::shared_ptr;

ClientGalaxy::ClientGalaxy() {
}


ClientGalaxy::~ClientGalaxy() {
}

void ClientGalaxy::createVertexArray() {
	vertexArray.setPrimitiveType(sf::Quads);
	vertexArray.resize(stars.size() * 4);
	int i = 0;
	for (shared_ptr<Star> s : stars) {
		sf::Vector2f offset1(s->radius, s->radius);
		sf::Vector2f offset2(-s->radius, s->radius);
		sf::Vertex* quad = &vertexArray[i * 4];
		quad[0].position = s->position - offset1; //Top Left
		quad[1].position = s->position + offset2; //Bottom Left
		quad[2].position = s->position + offset1; //Bottom Right
		quad[3].position = s->position - offset2; //Top Right
		//Set colour
		quad[0].color = s->colour;
		quad[1].color = s->colour;
		quad[2].color = s->colour;
		quad[3].color = s->colour;
		//Sneakily hide the offset from centre in texture coords
		quad[0].texCoords = sf::Vector2f(-1.0f, -1.0f);
		quad[1].texCoords = sf::Vector2f(-1.0f, 1.0f);
		quad[2].texCoords = sf::Vector2f(1.0f, 1.0f);
		quad[3].texCoords = sf::Vector2f(1.0f, -1.0f);
		i++;
	}
	//Load shaders
	if (!starShader.loadFromFile("shaders/star.frag", sf::Shader::Fragment)) {
		Console::log("Could not load star shader", Console::LogLevel::ERROR);
	}
}

void ClientGalaxy::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.shader = &starShader;
	target.draw(vertexArray, states);
}
