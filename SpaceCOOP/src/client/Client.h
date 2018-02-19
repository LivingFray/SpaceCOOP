#pragma once
#include <string>
#include <SFML\Network.hpp>
#include <thread>

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
	bool connected;
	void threadedReceive();
	void handlePacket(sf::Packet& packet);
};

