#pragma once
#include <SFML\Network.hpp>
#include <memory>
#include <thread>
#include <string>
#include "../shared/TSQueue.h"
#include "../shared/entities/EntityCore.h"
#include "../shared/entities/Ship.h"

using std::shared_ptr;
using std::thread;

class Server;

class Player :
	public std::enable_shared_from_this<Player> {
public:
	Player();
	~Player();
	void start();
	void disconnect();
	void sendText(std::string msg);
	void sendEntity(shared_ptr<EntityCore> entity);
	void updateEntity(sf::Packet p);
	void removeEntity(shared_ptr<EntityCore> entity);
	void removeAll();

	void warp(int destination);
	shared_ptr<Ship> ship;

	sf::IpAddress ip;
	unsigned short port;

	TSQueue<sf::Packet> toSendTCP;
	TSQueue<sf::Packet> toSendUDP;

	std::shared_ptr<Player> getptr() {
		return shared_from_this();
	}

private:
	shared_ptr<sf::TcpSocket> tcpSocket;
	sf::UdpSocket* udpSocket;
	thread receiveTCPThread;
	thread sendTCPThread;
	thread sendUDPThread;
	void threadedTCPReceive();
	void threadedTCPSend();
	void threadedUDPSend();
	bool running = false;
	void handlePacket(sf::Packet& packet);
	Server* server;
	friend class Server;
};

