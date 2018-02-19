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
private:
	sf::TcpSocket socket;
	sf::Thread receiveThread;
	bool connected;
	void threadedReceive();
};

