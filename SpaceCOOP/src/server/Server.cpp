#include "Server.h"
#include "../shared/Console.h"
#include "Player.h"
#include <algorithm>
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
#include "CommandLineConsole.h"

Server::Server() {
	console = std::make_shared<CommandLineConsole>();
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
		console->log("SV_Incoming: joined", Console::LogLevel::INFO);
	}
	if (udpThread.joinable()) {
		udpThread.join();
		console->log("SV_UDP: joined", Console::LogLevel::INFO);
	}
}


void Server::start() {
	if (running) {
		console->log("Attempted to start server that was already running", Console::LogLevel::ERROR);
	}
	running = true;
	lastSentPackets = 0;
	udpThread = std::thread(&Server::receiveUDP, this);
	console->log("SV_UDP: started", Console::LogLevel::INFO);
	console->log("Generating galaxy", Console::LogLevel::INFO);
	galaxy.server = this;
	galaxy.generateGalaxy();
	console->log("Galaxy generated", Console::LogLevel::INFO);
	incomingThread = std::thread(&Server::handleConnections, this);
	console->log("SV_Incoming: started", Console::LogLevel::INFO);
}


void Server::stop() {
	console->log(std::to_string(numPlayers) + " players connected", Console::LogLevel::INFO);
	running = false;
	listen.close();
	console->log("SV_Listen: closed", Console::LogLevel::INFO);
	incomingThread.join();
	console->log("SV_Incoming: joined", Console::LogLevel::INFO);
	udp.unbind();
	udpThread.join();
	console->log("SV_UDP: joined", Console::LogLevel::INFO);
	console->log("No longer listening for new connections", Console::LogLevel::INFO);
	for (shared_ptr<Player> t : players) {
		t->disconnect();
	}
	players.clear();
	console->log("All players kicked", Console::LogLevel::INFO);
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
		console->log(std::to_string(numPlayers) + " clients connected", Console::LogLevel::INFO);
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
	console->log("Warping to system " + std::to_string(destination), Console::LogLevel::INFO);
	galaxy.movePlayer(player, destination);
}

void Server::handleConnections() {
	listen.listen(port);
	console->log("SV_Listen: opened", Console::LogLevel::INFO);
	console->log("Started server", Console::LogLevel::INFO);
	while (running) {
		auto client = std::make_shared<sf::TcpSocket>();
		if (listen.accept(*client) == sf::TcpListener::Done) {
			//Handle max server capacity/nice logins and such here

			//Add client
			console->log("New client connected", Console::LogLevel::INFO);
			auto player = std::make_shared<Player>();
			//Pass socket to player
			player->tcpSocket = client;
			player->ip = client->getRemoteAddress();
			player->port = client->getRemotePort();
			player->udpSocket = &udp;
			console->log("PL_Socket: opened", Console::LogLevel::INFO);
			player->server = this;
			//Start listening
			player->start();
			//Track player in server
			players.push_back(player);
			numPlayers++;
			console->log(std::to_string(numPlayers) + " players connected", Console::LogLevel::INFO);
			onPlayerConnected(player);
		}
	}
	listen.close();
	console->log("SV_Listen: closed", Console::LogLevel::INFO);
}

void Server::receiveUDP() {
	auto status = udp.bind(port);
	if (status != sf::UdpSocket::Status::Done) {
		console->log("Error binding UDP socket to port " + std::to_string(port), Console::LogLevel::ERROR);
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
