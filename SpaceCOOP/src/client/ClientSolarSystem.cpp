#include "ClientSolarSystem.h"
#include "Client.h"
#include "commands/PreciseRotateClientCommand.h"
#include "../shared/Helper.h"


ClientSolarSystem::ClientSolarSystem() {
}


ClientSolarSystem::~ClientSolarSystem() {
}

void ClientSolarSystem::init() {
	background.client = client;
	background.generate();
	//Set view
	//TODO: Aspect radio stuff
	shipView.setSize(1600, 900);
	shipView.setCenter(0.0f, 0.0f);
	window = client->window;
}

void ClientSolarSystem::update(double dt) {
	SolarSystem::update(dt);
	//Maintain correct view for any coordinate changes
	window->setView(shipView);
	//Update player angles
	PreciseRotateClientCommand::time += dt;
	if (ship && orientToMouse) {
		sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		sf::Vector2f rel = mousePos - ship->getPosition();
		float desiredAngle = toDegrees(atan2f(rel.y, rel.x));
		PreciseRotateClientCommand cmd;
		cmd.client = client;
		cmd.angle = desiredAngle;
		cmd.execute();
		//TODO: Could send these less often by only sending if movement
	}
	//Update entities
	for (auto ent : entities) {
		ent.second->update(dt);
	}
	//Update projectiles
	for (auto proj = projectiles.begin(); proj != projectiles.end();) {
		(*proj)->update(dt);
		//Kill any old projectiles
		if ((*proj)->TTL < 0.0f) {
			proj = projectiles.erase(proj);
		} else {
			proj++;
		}
	}
}

void ClientSolarSystem::draw() {
	//Draw stars in background
	window->draw(background);
	//Reposition the camera to be on the ship
	if (ship) {
		shipView.setCenter(ship->getPosition());
	}
	//Draw the entities in this system
	window->setView(shipView);
	for (auto ent : entities) {
		window->draw(*ent.second);
	}
	//Draw projectiles
	for (auto proj : projectiles) {
		window->draw(*proj);
	}
	//Draw map of the solar system
	if (mapVisible) {
		window->setView(systemMapView);
		sf::RectangleShape backing;
		backing.setPosition(-maxSize / 2.0f, -maxSize / 2.0f);
		backing.setSize(sf::Vector2f(maxSize, maxSize));
		backing.setFillColor(sf::Color(0, 0, 53, 127));
		window->draw(backing);
		for (auto planet : planets) {
			sf::Vector2f scale = planet.second->getScale();
			planet.second->setScale(scale * 4.0f);
			window->draw(*planet.second);
			planet.second->setScale(scale);
		}
		if (star) {
			sf::Vector2f scale = star->getScale();
			star->setScale(scale * 4.0f);
			window->draw(*star);
			star->setScale(scale);
		}
		auto ship = client->getShip();
		if (ship) {
			sf::Vector2f scale = ship->getScale();
			ship->setScale(scale * 40.0f);
			window->draw(*ship);
			ship->setScale(scale);
		}
	}
}

void ClientSolarSystem::keyEvent(sf::Event e) {
}

void ClientSolarSystem::textEvent(sf::Event e) {
}

void ClientSolarSystem::resizeEvent(sf::Event e) {
	background.setAspect(e.size.width, e.size.height);
	resizeMap(e.size.width, e.size.height);
}

void ClientSolarSystem::addEntity(shared_ptr<EntityCore> entity) {
	entities.insert_or_assign(entity->id, entity);
	entity->system = this;
	if (std::dynamic_pointer_cast<Planet>(entity)) {
		addPlanet(std::dynamic_pointer_cast<Planet>(entity));
	}
	if (std::dynamic_pointer_cast<EntityStar>(entity)) {
		addStar(std::dynamic_pointer_cast<EntityStar>(entity));
	}
}

void ClientSolarSystem::removeEntity(UUID id) {
	if (!entities.erase(id)) {
		client->console.log("Could not find entity with UUID " + id, GraphicalConsole::LogLevel::WARNING);
	}
}

void ClientSolarSystem::removeAll() {
	entities.clear();
	//Reset solar system
	star = NULL;
	planets.clear();
	adjustMapView();
	//Clear pointer to ship, as its now gone
	ship = NULL;
}

void ClientSolarSystem::addPlanet(shared_ptr<Planet> planet) {
	SolarSystem::addPlanet(planet);
	adjustMapView();
}

void ClientSolarSystem::setMapVisibility(bool visible) {
	mapVisible = visible;
}

void ClientSolarSystem::adjustMapView() {
	maxSize = 1;
	//Calculate smallest square that covers entire system
	for (auto planet : planets) {
		sf::Vector2f pos = planet.second->getPosition();
		if (abs(pos.x) > maxSize) {
			maxSize = abs(pos.x);
		}
		if (abs(pos.y) > maxSize) {
			maxSize = abs(pos.y);
		}
	}
	//Fudge factor to give a bit of a border between furthest out and edge of map
	//Also doubles as set size takes total screen size, not size from centre
	maxSize *= 2.2f;
	float smallest = static_cast<float>(sWidth < sHeight ? sWidth : sHeight);
	systemMapView.setSize(maxSize * (sWidth / smallest), maxSize * (sHeight / smallest));
}

void ClientSolarSystem::resizeMap(unsigned int width, unsigned int height) {
	sWidth = width;
	sHeight = height;
	adjustMapView();
}
