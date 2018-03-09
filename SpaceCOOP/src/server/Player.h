#pragma once
#include <SFML\Network.hpp>
#include <memory>
#include <thread>
#include <string>
#include "../shared/TSQueue.h"
#include "../shared/entities/EntityCore.h"

using std::shared_ptr;
using std::thread;

class Server;

class Player {
public:
	Player();
	~Player();
	void start();
	void disconnect();
	void sendText(std::string msg);
	void sendEntity(shared_ptr<EntityCore> entity);
private:
	shared_ptr<sf::TcpSocket> socket;
	thread receiveThread;
	thread sendThread;
	void threadedReceive();
	void threadedSend();
	bool running = false;
	void handlePacket(sf::Packet& packet);
	TSQueue<sf::Packet> toSend;
	Server* server;
	friend class Server;
};

