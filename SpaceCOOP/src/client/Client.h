#pragma once
#include <string>
#include <SFML\Network.hpp>
#include <thread>
#include "../shared/TSQueue.h"

class Client {
public:
	Client();
	~Client();
	void connect();
	void disconnect();
	std::string ip;
	int port;
	void sendText(std::string msg);
private:
	sf::TcpSocket socket;
	std::thread receiveThread;
	std::thread sendThread;
	bool connected;
	void threadedReceive();
	void threadedSend();
	void handlePacket(sf::Packet& packet);
	TSQueue<sf::Packet> toSend;
};

