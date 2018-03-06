#pragma once
#include <SFML\Network.hpp>
#include <vector>
#include <memory>
#include <mutex>
#include "Player.h"
#include "ServerGalaxy.h"
#include <atomic>
#include "../shared/CommandHandler.h"

/*
Server is the main class for server side operations
1 Thread handles incoming connections
Each client has its own thread


*/

class Server {
public:
	Server();
	~Server();
	void start();
	void stop();
	void update(double dt);
	int MAX_PLAYERS = 1;
	int numPlayers = 0;
	int port;
	void updateConnectedList();
	const ServerGalaxy& getGalaxy();
	CommandHandler commandHandler;
private:
	std::thread incomingThread;
	sf::TcpListener listen;
	void handleConnections();
	bool running = false;
	std::vector<std::shared_ptr<Player>> players;
	std::atomic<bool> checkConnected = false;

	ServerGalaxy galaxy;
};

