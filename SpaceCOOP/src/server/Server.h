#pragma once
#include <SFML\Network.hpp>
#include <vector>
#include <memory>
#include "Player.h"

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
	int MAX_PLAYERS = 1;
	int numPlayers = 0;
	int port;
private:
	sf::Thread incomingThread;
	sf::TcpListener listen;
	void handleConnections();
	bool running = false;
	std::vector<std::shared_ptr<Player>> players;
};

