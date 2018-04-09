#include "ServerSolarSystem.h"
#include "Server.h"
#include "../shared/Helper.h"
#include "../shared/Console.h"
#include "../shared/PacketHandler.h"


ServerSolarSystem::ServerSolarSystem() {
}


ServerSolarSystem::~ServerSolarSystem() {
}


void ServerSolarSystem::generateSystem(Server* server) {
	//Random number of planets from 1 to 12
	std::uniform_int_distribution<int> intDist(1, 12);
	//Random position of planets in orbit
	std::uniform_real_distribution<float> angDist(0.0f, toRadians(360.0f));
	//Random altitude of orbits
	std::uniform_real_distribution<float> orbitDist(5000.0f, 30000.0f);
	//Random radius of planet
	std::uniform_real_distribution<float> radiusDist(50.0f, 500.0f);
	float previousOrbit = 0.0f; // Distance (in AU) the previous planet was from the star
	for (int i = 0; i < intDist(generator); i++) {
		//Move out and put planet at random rotation around star
		float orbit = previousOrbit + orbitDist(generator);
		previousOrbit = orbit;
		float angle = angDist(generator);
		sf::Vector2f planetPosition(cosf(angle), sinf(angle));
		planetPosition *= orbit;
		//Create new planet
		shared_ptr<Planet> planet = std::make_shared<Planet>(radiusDist(generator));
		planet->setPosition(planetPosition);
		planets.push_back(planet);
		//Register planet in server entity list (May wish to change later, or add reference to system in planet to allow for multiple systems loaded at once)
		//server->addEntity(planet);
		addEntity(planet);
	}
}


void ServerSolarSystem::addEntity(shared_ptr<EntityCore> entity) {
	entityLock.lock();
	entities.insert_or_assign(entity->id, entity);
	entityLock.unlock();
	playerLock.lock();
	for (auto player : players) {
		player->sendEntity(entity);
	}
	playerLock.unlock();
}

shared_ptr<EntityCore> ServerSolarSystem::getEntity(UUID id) {
	std::lock_guard<std::mutex> guard(entityLock);
	return entities[id];
}

void ServerSolarSystem::removeEntity(UUID id) {
	entityLock.lock();
	auto ent = entities[id];
	if (!ent) {
		Console::logToConsole("Could not find entity with UUID " + id, Console::LogLevel::WARNING);
		entityLock.unlock();
		return;
	}
	entities.erase(id);
	entityLock.unlock();
	playerLock.lock();
	for (auto player : players) {
		player->removeEntity(ent);
	}
	playerLock.unlock();
}

void ServerSolarSystem::addPlayer(shared_ptr<Player> player) {
	//Add player to list
	playerLock.lock();
	players.push_back(player);
	playerLock.unlock();
	//Create ship
	shared_ptr<Ship> playerShip = std::make_shared<Ship>();
	player->ship = playerShip;
	//Send ship to players
	addEntity(playerShip);
	//Assign ship to player
	sf::Packet p;
	p << static_cast<sf::Uint8>(PacketHandler::Type::ASSIGN_SHIP);
	p << playerShip->id;
	player->toSendTCP.push(p);
	//Send entities to new player
	entityLock.lock();
	for (auto ent : entities) {
		if (ent.first != playerShip->id) {
			player->sendEntity(ent.second);
		}
	}
	entityLock.unlock();
}

void ServerSolarSystem::removePlayer(shared_ptr<Player> player) {
	//Remove player from player list
	playerLock.lock();
	for (auto it = players.begin(); it != players.end();) {
		if (*it == player) {
			it = players.erase(it);
		} else {
			it++;
		}
	}
	playerLock.unlock();
	//Tell player to remove entities
	player->removeAll();
	//Remove ship entity
	if (player->ship) {
		removeEntity(player->ship->id);
	}
}

void ServerSolarSystem::update(double dt) {
	//Update entities
	entityLock.lock();
	for (auto ent : entities) {
		ent.second->update(dt);
	}
	entityLock.unlock();
}
