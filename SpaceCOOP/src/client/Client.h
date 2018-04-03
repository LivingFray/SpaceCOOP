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
#include "../shared/entities/Ship.h"
#include "GraphicalConsole.h"

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
	unsigned short port;
	void sendText(std::string msg);
	void sendCommand(ClientCommand* cmd);
	void draw();
	void update(double dt);
	void removeEntity(UUID id);
	void showConsole();

	void keyEvent(sf::Event e);
	void textEvent(sf::Event e);
	void resizeEvent(sf::Event e);

	sf::RenderWindow* window;
	CommandHandler commandHandler;
	InputHandler inputHandler;
	EntityHandler entityHandler;
	shared_ptr<Ship> ship;
	bool consoleVisible = false;
private:
	sf::TcpSocket tcpSocket;
	sf::UdpSocket udpSocket;
	thread receiveTCPThread;
	thread sendTCPThread;
	thread receiveUDPThread;
	thread sendUDPThread;
	bool connected = false;
	void threadedTCPReceive();
	void threadedTCPSend();
	void threadedUDPReceive();
	void threadedUDPSend();
	void handlePacket(sf::Packet& packet);
	TSQueue<sf::Packet> tcpToSend;
	TSQueue<sf::Packet> udpToSend;
	sf::IpAddress addr;

	GraphicalConsole console;
	ClientGalaxy galaxy;
	unordered_map<UUID, shared_ptr<EntityCore>> entities;
	bool consoleJustVisible = false;
	bool orientToMouse = true;
};

