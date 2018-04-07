#include "Background.h"
#include "../shared/Helper.h"
#include "Client.h"
#define MIN_STARS 1500
#define MAX_STARS 2000
#define MIN_RADIUS 1.25f
#define MAX_RADIUS 2.5f
#define SIZE 1024.0f

Background::Background() {
}


Background::~Background() {
}

void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.setView(backgroundView);
	states.shader = &starShader;
	target.draw(starVerts, states);
}

void Background::generate() {
	int numStars = MIN_STARS + rand() % (MAX_STARS - MIN_STARS);
	//Generate square of stars and such
	starVerts.setPrimitiveType(sf::Quads);
	starVerts.resize(numStars * 4);
	for (int i = 0; i < numStars; i++) {
		float radius = randF(MIN_RADIUS, MAX_RADIUS);
		sf::Vector2f offset1(radius, radius);
		sf::Vector2f offset2(-radius, radius);
		sf::Vertex* quad = &starVerts[i * 4];
		//Very janky way of ensuring stars appear random, but not clumped
		//Stupid human brain and its pattern finding abilities ruining randomness
		sf::Vector2f position;
		position.x = static_cast<float>(i % static_cast<int>(sqrt(numStars)));
		position.y = static_cast<float>(i / static_cast<int>(sqrt(numStars)));
		position.x /= static_cast<int>(sqrt(numStars));
		position.y /= static_cast<int>(sqrt(numStars));
		position.x *= SIZE;
		position.y *= SIZE;
		float boxSize = SIZE / static_cast<int>(sqrt(numStars));
		//4 is just a fudge factor that allows for some overlap, hiding the star grid
		position.x += 4 * randF(-boxSize / 2, boxSize / 2);
		position.y += 4 * randF(-boxSize / 2, boxSize / 2);
		quad[0].position = position - offset1; //Top Left
		quad[1].position = position + offset2; //Bottom Left
		quad[2].position = position + offset1; //Bottom Right
		quad[3].position = position - offset2; //Top Right
		//Set colour
		sf::Color colour;
		float r, g, b;
		bv2rgb(r, g, b, randF(-0.4f, 2.0f));
		colour.r = static_cast<sf::Uint8>(r * 255.0f);
		colour.g = static_cast<sf::Uint8>(g * 255.0f);
		colour.b = static_cast<sf::Uint8>(b * 255.0f);
		quad[0].color = colour;
		quad[1].color = colour;
		quad[2].color = colour;
		quad[3].color = colour;
		//Sneakily hide the offset from centre in texture coords
		quad[0].texCoords = sf::Vector2f(-1.0f, -1.0f);
		quad[1].texCoords = sf::Vector2f(-1.0f, 1.0f);
		quad[2].texCoords = sf::Vector2f(1.0f, 1.0f);
		quad[3].texCoords = sf::Vector2f(1.0f, -1.0f);
	}
	//Load shaders TODO: Asset handler for loading shaders once
	if (!starShader.loadFromFile("shaders/star.frag", sf::Shader::Fragment)) {
		client->console.log("Could not load star shader", GraphicalConsole::LogLevel::ERROR);
	}
}

void Background::setAspect(unsigned int width, unsigned int height) {
	/*
	Get desired box size (largest of width / height)
	Largest side has size of largest/largest
	Smaller side has size of smaller/largest

	*/
	float largest = static_cast<float>(width > height ? width : height);
	float x = SIZE * (width / largest);
	float y = SIZE * (height / largest);
	backgroundView.setSize(x, y);
	backgroundView.setCenter(x / 2, y / 2);
}
