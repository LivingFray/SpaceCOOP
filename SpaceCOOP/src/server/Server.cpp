#include "Server.h"
#include "../shared/Console.h"
#include "Player.h"
#include <algorithm>
#include <mutex>
#include "../shared/CommandHandler.h"
#include "../shared/PacketHandler.h"
#include "commands/ForwardsServerCommand.h"
#include "commands/BackwardsServerCommand.h"
#include "commands/StrafeLeftServerCommand.h"
#include "commands/StrafeRightServerCommand.h"
#include "commands/RotateLeftServerCommand.h"
#include "commands/RotateRightServerCommand.h"
#include "commands/PreciseRotateServerCommand.h"
#include "commands/WarpServerCommand.h"
#include "commands/FireServerCommand.h"
//TODO: Server side ship with movement and such
#include "../shared/entities/Ship.h"
#include "../shared/Helper.h"

Server::Server() {
	//Register all commands here
	REGCMD(ForwardsServerCommand);
	REGCMD(BackwardsServerCommand);
	REGCMD(StrafeLeftServerCommand);
	REGCMD(StrafeRightServerCommand);
	REGCMD(RotateLeftServerCommand);
	REGCMD(RotateRightServerCommand);
	REGCMD(PreciseRotateServerCommand);
	REGCMD(WarpServerCommand);
	REGCMD(FireServerCommand);
}


Server::~Server() {
	running = false;
	if (incomingThread.joinable()) {
		incomingThread.join();
		Console::logToConsole("SV_Incoming: joined", Console::LogLevel::INFO);
	}
	if (udpThread.joinable()) {
		udpThread.join();
		Console::logToConsole("SV_UDP: joined", Console::LogLevel::INFO);
	}
}


void Server::start() {
	if (running) {
		Console::logToConsole("Attempted to start server that was already running", Console::LogLevel::ERROR);
	}
	running = true;
	lastSentPackets = 0;
	udpThread = std::thread(&Server::receiveUDP, this);
	Console::logToConsole("SV_UDP: started", Console::LogLevel::INFO);
	Console::logToConsole("Generating galaxy", Console::LogLevel::INFO);
	galaxy.generateGalaxy();
	Console::logToConsole("Galaxy generated", Console::LogLevel::INFO);
	incomingThread = std::thread(&Server::handleConnections, this);
	Console::logToConsole("SV_Incoming: started", Console::LogLevel::INFO);
}


void Server::stop() {
	Console::logToConsole(std::to_string(numPlayers) + " players connected", Console::LogLevel::INFO);
	running = false;
	listen.close();
	Console::logToConsole("SV_Listen: closed", Console::LogLevel::INFO);
	incomingThread.join();
	Console::logToConsole("SV_Incoming: joined", Console::LogLevel::INFO);
	udp.unbind();
	udpThread.join();
	Console::logToConsole("SV_UDP: joined", Console::LogLevel::INFO);
	Console::logToConsole("No longer listening for new connections", Console::LogLevel::INFO);
	for (shared_ptr<Player> t : players) {
		t->disconnect();
	}
	players.clear();
	Console::logToConsole("All players kicked", Console::LogLevel::INFO);
}

void Server::update(double dt) {
	//Handle disconnects
	if (checkConnected) {
		auto it = players.begin();
		while (it != players.end()) {
			if (!(*it)->running) {
				onPlayerDisconnected(*it);
				it = players.erase(it);
				numPlayers--;
			} else {
				it++;
			}
		}
		checkConnected = false;
		Console::logToConsole(std::to_string(numPlayers) + " clients connected", Console::LogLevel::INFO);
	}
	galaxy.updateSystems(dt);
	//Send packets if it is that time again
	lastSentPackets += dt;
	if (lastSentPackets > packetRate) {
		lastSentPackets -= packetRate;
		galaxy.sendUpdates();
	}
}

void Server::updateConnectedList() {
	checkConnected = true;
}

const ServerGalaxy& Server::getGalaxy() {
	return galaxy;
}

void Server::onPlayerConnected(shared_ptr<Player> player) {
	//TODO: Handle this properly
	galaxy.addPlayer(player);
}

void Server::onPlayerDisconnected(shared_ptr<Player> player) {
	galaxy.removePlayer(player);
}

void Server::warp(shared_ptr<Player> player, int destination) {
	Console::logToConsole("Warping to system " + std::to_string(destination), Console::LogLevel::INFO);
	galaxy.movePlayer(player, destination);
}

void Server::handleConnections() {
	listen.listen(port);
	Console::logToConsole("SV_Listen: opened", Console::LogLevel::INFO);
	Console::logToConsole("Started server", Console::LogLevel::INFO);
	while (running) {
		auto client = std::make_shared<sf::TcpSocket>();
		if (listen.accept(*client) == sf::TcpListener::Done) {
			//Handle max server capacity/nice logins and such here

			//Add client
			Console::logToConsole("New client connected", Console::LogLevel::INFO);
			auto player = std::make_shared<Player>();
			//Pass socket to player
			player->tcpSocket = client;
			player->ip = client->getRemoteAddress();
			player->port = client->getRemotePort();
			player->udpSocket = &udp;
			Console::logToConsole("PL_Socket: opened", Console::LogLevel::INFO);
			player->server = this;
			//Start listening
			player->start();
			//Track player in server
			players.push_back(player);
			numPlayers++;
			Console::logToConsole(std::to_string(numPlayers) + " players connected", Console::LogLevel::INFO);
			onPlayerConnected(player);
		}
	}
	listen.close();
	Console::logToConsole("SV_Listen: closed", Console::LogLevel::INFO);
}

void Server::receiveUDP() {
	auto status = udp.bind(port);
	if (status != sf::UdpSocket::Status::Done) {
		Console::logToConsole("Error binding UDP socket to port " + std::to_string(port), Console::LogLevel::ERROR);
	}
	sf::Packet packet;
	sf::IpAddress fromAddr;
	unsigned short fromPort;
	while (running) {
		udp.receive(packet, fromAddr, fromPort);
		for (auto p : players) {
			if (p->ip == fromAddr && p->port == fromPort) {
				p->handlePacket(packet);
			}
		}
	}
}
