#include "Server.h"
#include "../shared/Console.h"
#include "Player.h"
#include <algorithm>
#include <mutex>

Server::Server() {
}


Server::~Server() {
	if (incomingThread.joinable()) {
		running = false;
		incomingThread.join();
		Console::log("SV_Incoming: joined", Console::LogLevel::INFO);
	}
}


void Server::start() {
	if (running) {
		Console::log("Attempted to start server that was already running", Console::LogLevel::ERROR);
	}
	running = true;
	incomingThread = std::thread(&Server::handleConnections, this);
	Console::log("SV_Incoming: started", Console::LogLevel::INFO);
	Console::log("Generating galaxy", Console::LogLevel::INFO);
	galaxy.generateGalaxy();
	Console::log("Galaxy generated", Console::LogLevel::INFO);
}


void Server::stop() {
	Console::log(std::to_string(numPlayers) + " players connected", Console::LogLevel::INFO);
	running = false;
	listen.close();
	Console::log("SV_Listen: closed", Console::LogLevel::INFO);
	incomingThread.join();
	Console::log("SV_Incoming: joined", Console::LogLevel::INFO);
	Console::log("No longer listening for new connections", Console::LogLevel::INFO);
	for (std::shared_ptr<Player> t : players) {
		t->disconnect();
	}
	players.clear();
	Console::log("All players kicked", Console::LogLevel::INFO);
}

void Server::update(double dt) {
	if (checkConnected) {
		auto it = players.begin();
		while (it != players.end()) {
			if (!(*it)->running) {
				it = players.erase(it);
				numPlayers--;
			} else {
				it++;
			}
		}
		checkConnected = false;
		Console::log(std::to_string(numPlayers) + " clients connected", Console::LogLevel::INFO);
	}
}

void Server::updateConnectedList() {
	checkConnected = true;
}

const ServerGalaxy& Server::getGalaxy() {
	return galaxy;
}


void Server::handleConnections() {
	listen.listen(port);
	Console::log("SV_Listen: opened", Console::LogLevel::INFO);
	Console::log("Started server", Console::LogLevel::INFO);
	while (running) {
		auto client = std::make_shared<sf::TcpSocket>();
		if (listen.accept(*client) == sf::TcpListener::Done) {
			//Handle max server capacity/nice logins and such here

			//Add client
			Console::log("New client connected", Console::LogLevel::INFO);
			auto player = std::make_shared<Player>();
			//Pass socket to player
			player->socket = client;
			Console::log("PL_Socket: opened", Console::LogLevel::INFO);
			player->server = this;
			//Start listening
			player->start();
			//Track player in server
			players.push_back(player);
			numPlayers++;
			Console::log(std::to_string(numPlayers) + " players connected", Console::LogLevel::INFO);
		}
	}
	listen.close();
	Console::log("SV_Listen: closed", Console::LogLevel::INFO);
}
