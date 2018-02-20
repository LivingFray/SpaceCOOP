#pragma once
#include <SFML\Network.hpp>
#include <memory>
#include <thread>

class Server;

class Player {
public:
	Player();
	~Player();
	void start();
	void disconnect();
private:
	std::shared_ptr<sf::TcpSocket> socket;
	std::thread receiveThread;
	void receive();
	bool running = false;
	void handlePacket(sf::Packet& packet);
	Server* server;
	friend class Server;
};

