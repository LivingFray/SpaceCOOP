#pragma once
#include "../shared/SolarSystem.h"
#include "../shared/entities/EntityCore.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <mutex>
#include "Player.h"
#include "AI.h"
#include "../shared/Projectile.h"
#include "../shared/Console.h"

using std::unordered_map;
using std::shared_ptr;
using std::vector;
using std::mutex;

class Server;

class ServerSolarSystem :
	public SolarSystem {
public:
	ServerSolarSystem();
	~ServerSolarSystem();
	void generateSystem(Server* server);

	void addEntity(shared_ptr<EntityCore> entity);
	void removeEntity(UUID id);

	void addPlayer(shared_ptr<Player> player);
	void removePlayer(shared_ptr<Player> player);

	void update(double dt);
	void sendUpdates();

	void addProjectile(shared_ptr<Projectile> proj);

	vector<shared_ptr<Player>> getPlayers() { return players; };

	shared_ptr<Console> getConsole() { return console; }

	static std::default_random_engine generator;

	mutex playerLock;

	sf::Color starColour;
private:
	Server* server;
	shared_ptr<Console> console;
	vector<shared_ptr<Player>> players;
	vector<shared_ptr<AI>> ais;
};

