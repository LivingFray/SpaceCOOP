#pragma once
#include <SFML\Network.hpp>
#include <memory>

class Server;

class Player {
public:
	Player();
	~Player();
	void start();
	void disconnect();
private:
	std::shared_ptr<sf::TcpSocket> socket;
	sf::Thread receiveThread;
	void receive();
	bool running = false;
	void handlePacket(sf::Packet& packet);
	friend class Server;
};

