#pragma once
#include <SFML\Network.hpp>
#include <list>

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
	sf::Thread receiveThread;
	bool running = false;
	void threadedReceive();
};

