#pragma once
#include <SFML/Graphics.hpp>
#include "../shared/SolarSystem.h"

class Client;

class ClientSolarSystem :
	public SolarSystem {
public:
	ClientSolarSystem();
	~ClientSolarSystem();
	void init();
	void addPlanet(shared_ptr<Planet> planet);

	void draw() const;
	void resize(unsigned int width, unsigned int height);
	bool mapVisible = false;

	void reset();

	Client* client;
private:
	sf::View view;
	float maxSize;
	sf::RenderWindow* window;
	unsigned int sWidth, sHeight;
	void adjustView();
	//TEMP: Star should be entity
	//sf::Shader starShader;
};

