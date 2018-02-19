#pragma once
#include <string>
#include <SFML\Network.hpp>

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
	sf::Thread receiveThread;
	bool connected;
	void threadedReceive();
	void handlePacket(sf::Packet& packet);
};

