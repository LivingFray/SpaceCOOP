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
#include "Scene.h"

using std::thread;
using std::shared_ptr;
using std::unordered_map;

class ClientCommand;

class Client {
public:
	Client();
	~Client();
	void init();
	void connect();
	void disconnect();
	std::string ip;
	unsigned short port;
	void sendText(std::string msg);
	void sendCommand(ClientCommand* cmd);
	void draw();
	void update(double dt);
	void addEntity(shared_ptr<EntityCore> entity);
	shared_ptr<EntityCore> getEntity(UUID id);
	void removeEntity(UUID id);
	void removeAll();
	void showConsole();
	void setSystemMapVisibility(bool visible);
	shared_ptr<Ship> getShip();

	void keyEvent(sf::Event e);
	void textEvent(sf::Event e);
	void resizeEvent(sf::Event e);

	sf::RenderWindow* window;
	CommandHandler commandHandler;
	InputHandler inputHandler;
	EntityHandler entityHandler;
	bool consoleVisible = false;
	GraphicalConsole console;
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

	ClientGalaxy galaxy;
	bool consoleJustVisible = false;

	Scene scene;
};

