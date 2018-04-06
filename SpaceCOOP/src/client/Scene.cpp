#include "Scene.h"
#include "Client.h"
#include "commands/PreciseRotateClientCommand.h"
#include "../shared/Helper.h"


Scene::Scene() {
}


Scene::~Scene() {
}

void Scene::init() {
	background.client = client;
	background.generate();
	//Set view
	//TODO: Aspect radio stuff
	shipView.setSize(1600, 900);
	shipView.setCenter(0.0f, 0.0f);
	window = client->window;
}

void Scene::update(double dt) {
	//Update player angles
	if (ship && orientToMouse) {
		sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
		sf::Vector2f rel = mousePos - ship->getPosition();
		float desiredAngle = toDegrees(atan2f(rel.y, rel.x));
		PreciseRotateClientCommand cmd;
		cmd.client = client;
		cmd.angle = desiredAngle;
		cmd.execute();
	}
	for (auto ent : entities) {
		ent.second->update(dt);
	}
}

void Scene::draw() {
	window->draw(background);
	if (ship) {
		shipView.setCenter(ship->getPosition());
	}
	window->setView(shipView);
	for (auto ent : entities) {
		window->draw(*ent.second);
	}
}

void Scene::keyEvent(sf::Event e) {
}

void Scene::textEvent(sf::Event e) {
}

void Scene::resizeEvent(sf::Event e) {
	background.setAspect(e.size.width, e.size.height);
}

void Scene::addEntity(shared_ptr<EntityCore> entity) {
	entities.insert_or_assign(entity->id, entity);
}

shared_ptr<EntityCore> Scene::getEntity(UUID id) {
	return entities[id];
}

void Scene::removeEntity(UUID id) {
	if (!entities.erase(id)) {
		client->console.log("Could not find entity with UUID " + id, GraphicalConsole::LogLevel::WARNING);
	}
}
