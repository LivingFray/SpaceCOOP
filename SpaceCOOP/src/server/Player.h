#pragma once
#include <SFML\Network.hpp>
#include <memory>
#include <thread>
#include <string>
#include "../shared/TSQueue.h"

class Server;

class Player {
public:
	Player();
	~Player();
	void start();
	void disconnect();
	void sendText(std::string msg);
private:
	std::shared_ptr<sf::TcpSocket> socket;
	std::thread receiveThread;
	std::thread sendThread;
	void threadedReceive();
	void threadedSend();
	bool running = false;
	void handlePacket(sf::Packet& packet);
	TSQueue<sf::Packet> toSend;
	Server* server;
	friend class Server;
};

