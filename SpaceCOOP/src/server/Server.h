#pragma once
#include <SFML\Network.hpp>
#include <vector>
#include <memory>
#include <mutex>
#include "Player.h"
#include "ServerGalaxy.h"
#include <atomic>
#include "../shared/CommandHandler.h"
#include "../shared/EntityHandler.h"
#include <unordered_map>
#include "../shared/entities/EntityCore.h"
#include "ServerSolarSystem.h"

using std::vector;
using std::shared_ptr;
using std::thread;

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
	EntityHandler entityHandler;
	void onPlayerConnected(shared_ptr<Player> player);
	void onPlayerDisconnected(shared_ptr<Player> player);

	void warp(shared_ptr<Player> player, int destination);
private:
	thread incomingThread;
	thread udpThread;
	sf::TcpListener listen;
	sf::UdpSocket udp;
	void handleConnections();
	void receiveUDP();
	bool running = false;
	vector<shared_ptr<Player>> players;
	std::atomic<bool> checkConnected = false;

	ServerGalaxy galaxy;

	double const packetRate = 1.0/15.0;
	double lastSentPackets = 0;
};

