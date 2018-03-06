#pragma once
#include <string>
#include <SFML\Network.hpp>
#include <thread>
#include "../shared/TSQueue.h"
#include "ClientGalaxy.h"
#include "../shared/CommandHandler.h"
#include "ClientCommand.h"
#include "InputHandler.h"

class ClientCommand;

class Client {
public:
	Client();
	~Client();
	void connect();
	void disconnect();
	std::string ip;
	int port;
	void sendText(std::string msg);
	void sendCommand(ClientCommand* cmd);
	void draw();

	sf::RenderWindow* window;
	CommandHandler commandHandler;
	InputHandler inputHandler;
private:
	sf::TcpSocket socket;
	std::thread receiveThread;
	std::thread sendThread;
	bool connected;
	void threadedReceive();
	void threadedSend();
	void handlePacket(sf::Packet& packet);
	TSQueue<sf::Packet> toSend;

	ClientGalaxy galaxy;
};

