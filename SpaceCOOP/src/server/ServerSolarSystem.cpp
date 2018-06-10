#include "ServerSolarSystem.h"
#include "Server.h"
#include "../shared/Helper.h"
#include "../shared/Console.h"
#include "../shared/PacketHandler.h"
#include "../shared/entities/EntityStar.h"
#include "SampleAI.h"

std::default_random_engine ServerSolarSystem::generator;

ServerSolarSystem::ServerSolarSystem() {
}


ServerSolarSystem::~ServerSolarSystem() {
}
//TODO: Reduce uses of mutex by putting new players in a queue and only manipulating player list in main thread

void ServerSolarSystem::generateSystem(Server* server) {
	this->server = server;
	console = server->getConsole();
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
		addPlanet(planet);
		addEntity(planet);
	}
	console->log("Generated " + std::to_string(toAddPlanets.size()) + " planets", Console::LogLevel::INFO);
	//TODO: Binary systems
	star = std::make_shared<EntityStar>(radiusDist(generator) * 5.0f);
	star->setPosition(sf::Vector2f(0.0f, 0.0f));
	star->setColour(starColour);
	addEntity(star);

	//Update solar system
	updateToAdds();
}


void ServerSolarSystem::addEntity(shared_ptr<EntityCore> entity) {
	SolarSystem::addEntity(entity);
	entity->setSolarSystem(this);
	playerLock.lock();
	for (auto player : players) {
		player->sendEntity(entity);
	}
	playerLock.unlock();
}

void ServerSolarSystem::removeEntity(UUID id) {
	SolarSystem::removeEntity(id);
	playerLock.lock();
	for (auto player : players) {
		player->removeEntity(id);
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
	console->log(std::to_string(entities.size()), Console::LogLevel::INFO);
	for (auto ent : entities) {
		if (ent.first != playerShip->id) {
			player->sendEntity(ent.second);
		}
	}
}

void ServerSolarSystem::removePlayer(shared_ptr<Player> player) {
	bool removed = false;
	//Remove player from player list
	playerLock.lock();
	for (auto it = players.begin(); it != players.end();) {
		if (*it == player) {
			it = players.erase(it);
			removed = true;
		} else {
			it++;
		}
	}
	playerLock.unlock();
	//Tell player to remove entities
	if (removed) {
		player->removeAll();
		//Remove ship entity
		if (player->ship) {
			removeEntity(player->ship->id);
		}
	}
}

void ServerSolarSystem::update(double dt) {
	SolarSystem::update(dt);
	playerLock.lock();
	int numPlayers = players.size();
	playerLock.unlock();
	//Why update a system thats not occupied?
	if (numPlayers == 0) {
		return;
	}
	//TEMP
	if (ais.size() == 0) {
		shared_ptr<Ship> aiShip = std::make_shared<Ship>();
		addEntity(aiShip);
		ais.push_back(std::make_shared<SampleAI>(this, aiShip));
	}
	//Update AIs
	for (auto ai : ais) {
		ai->update(dt);
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

void ServerSolarSystem::sendUpdates() {
	for (auto tuple : entities) {
		auto ent = tuple.second;
		sf::Packet p;
		ent->generateModifyPacket(p);
		if (p.getDataSize() > 0) {
			sf::Packet sendPacket;
			sendPacket << ent->id;
			concatPackets(sendPacket, p);
			playerLock.lock();
			for (auto player : players) {
				player->updateEntity(sendPacket);
			}
			playerLock.unlock();
		}
	}
}

void ServerSolarSystem::addProjectile(shared_ptr<Projectile> proj) {
	SolarSystem::addProjectile(proj);
	sf::Packet p;
	p << static_cast<sf::Uint8>(PacketHandler::Type::PROJECTILE);
	p << *proj;
	playerLock.lock();
	for (auto player : players) {
		player->toSendUDP.push(p);
	}
	playerLock.unlock();
}
