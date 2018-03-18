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
//TODO: Server side ship with movement and such
#include "../shared/entities/Ship.h"
#include "../shared/Helper.h"

Server::Server() {
	//Register all commands here
	REGCMD(ForwardsServerCommand, 0);
	REGCMD(BackwardsServerCommand, 1);
	REGCMD(StrafeLeftServerCommand, 2);
	REGCMD(StrafeRightServerCommand, 3);
	REGCMD(RotateLeftServerCommand, 4);
	REGCMD(RotateRightServerCommand, 5);
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
	lastSentPackets = 0;
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
	for (shared_ptr<Player> t : players) {
		t->disconnect();
	}
	players.clear();
	Console::log("All players kicked", Console::LogLevel::INFO);
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
		Console::log(std::to_string(numPlayers) + " clients connected", Console::LogLevel::INFO);
	}
	//Update entities
	for (auto ent : entities) {
		ent.second->update(dt);
	}

	//Send packets if it is that time again
	lastSentPackets += dt;
	if (lastSentPackets > packetRate) {
		lastSentPackets -= packetRate;
		for (auto tuple : entities) {
			auto ent = tuple.second;
			sf::Packet p;
			ent->generateModifyPacket(p);
			if (p.getDataSize() > 0) {
				sf::Packet sendPacket;
				sendPacket << ent->id;
				concatPackets(sendPacket, p);
				for (auto player : players) {
					player->updateEntity(sendPacket);
				}
			}
		}
	}
}

void Server::updateConnectedList() {
	checkConnected = true;
}

const ServerGalaxy& Server::getGalaxy() {
	return galaxy;
}

void Server::onPlayerConnected(shared_ptr<Player> player) {
	//Add player's ship
	shared_ptr<Ship> playerShip = std::make_shared<Ship>();
	player->ship = playerShip;
	//Send ship to clients
	addEntity(playerShip);
	sf::Packet p;
	p << static_cast<sf::Uint8>(PacketHandler::Type::ASSIGN_SHIP);
	p << playerShip->id;
	player->toSend.push(p);
	//Send all entities to client
}

void Server::onPlayerDisconnected(shared_ptr<Player> player) {
	//Remove player's ship
}

void Server::addEntity(shared_ptr<EntityCore> entity) {
	UUID id = entity->id;
	if (entities[id]) {
		Console::log("Could not create entity, UUID already in use", Console::LogLevel::ERROR);
		return;
	}
	entities[id] = entity;
	for (auto player : players) {
		player->sendEntity(entity);
	}

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
			onPlayerConnected(player);
		}
	}
	listen.close();
	Console::log("SV_Listen: closed", Console::LogLevel::INFO);
}
