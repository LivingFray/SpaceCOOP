#pragma once
#include <string>
#include <SFML\Network.hpp>
#include <thread>
#include "../shared/TSQueue.h"
#include "ClientGalaxy.h"
#include "../shared/CommandHandler.h"
#include "commands/ClientCommand.h"
#include "InputHandler.h"
#include "../shared/EntityHandler.h"

using std::thread;
using std::shared_ptr;
using std::unordered_map;

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
	EntityHandler entityHandler;
private:
	sf::TcpSocket socket;
	thread receiveThread;
	thread sendThread;
	bool connected;
	void threadedReceive();
	void threadedSend();
	void handlePacket(sf::Packet& packet);
	TSQueue<sf::Packet> toSend;

	ClientGalaxy galaxy;
	unordered_map<UUID, shared_ptr<EntityCore>> entities;
};

