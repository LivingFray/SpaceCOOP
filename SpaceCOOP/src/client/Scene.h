#pragma once
#include "Background.h"
#include <memory>
#include <unordered_map>
#include "../shared/entities/EntityCore.h"
#include "../shared/entities/Ship.h"
#include "ClientSolarSystem.h"

using std::shared_ptr;
using std::unordered_map;

class Client;

class Scene {
public:
	Scene();
	~Scene();
	void init();
	void update(double dt);
	void draw();

	void keyEvent(sf::Event e);
	void textEvent(sf::Event e);
	void resizeEvent(sf::Event e);

	Client* client;
	shared_ptr<Ship> ship;

	void addEntity(shared_ptr<EntityCore> entity);
	shared_ptr<EntityCore> getEntity(UUID id);
	void removeEntity(UUID id);
	void removeAll();

	void setMapVisibility(bool visible);
private:
	unordered_map<UUID, shared_ptr<EntityCore>> entities;
	sf::View shipView;
	Background background;
	ClientSolarSystem solarSystem;
	sf::RenderWindow* window;
	bool orientToMouse = true;
};

