#pragma once
#include "Background.h"
#include <memory>
#include <unordered_map>
#include "../shared/entities/EntityCore.h"
#include "../shared/entities/Ship.h"
#include "../shared/SolarSystem.h"
#include "../shared/Projectile.h"

using std::shared_ptr;
using std::unordered_map;

class Client;

class ClientSolarSystem :
	public SolarSystem {
public:
	ClientSolarSystem();
	~ClientSolarSystem();
	void init();
	void update(double dt);
	void draw();

	void keyEvent(sf::Event e);
	void textEvent(sf::Event e);
	void resizeEvent(sf::Event e);

	Client* client;
	shared_ptr<Ship> ship;

	void addEntity(shared_ptr<EntityCore> entity);
	void removeEntity(UUID id);
	void removeAll();
	void addPlanet(shared_ptr<Planet> planet);

	void setMapVisibility(bool visible);
private:
	sf::View shipView;
	Background background;
	sf::RenderWindow* window;
	bool orientToMouse = true;
	sf::View systemMapView;
	float maxSize;
	unsigned int sWidth, sHeight;
	void adjustMapView();
	bool mapVisible = false;
	void resizeMap(unsigned int width, unsigned int height);
	bool mapNeedsUpdate = false;
};

