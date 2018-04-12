#pragma once
#include "..\shared\Galaxy.h"
#include <vector>
#include <memory>
#include <mutex>
#include "ServerSolarSystem.h"

class Server;

using std::vector;
using std::shared_ptr;
using std::mutex;

class ServerGalaxy :
	public Galaxy {
public:
	ServerGalaxy();
	~ServerGalaxy();

	//TODO: Save/Load
	void generateGalaxy();
	void generateSystem(int id);

	void updateSystems(double dt);
	void sendUpdates();

	void addPlayer(shared_ptr<Player> player);
	void movePlayer(shared_ptr<Player> player, int systemNum);
	void removePlayer(shared_ptr<Player> player);

	Server* server;

private:
	unordered_map<int, shared_ptr<ServerSolarSystem>> systems;

	mutex systemLock;
};

