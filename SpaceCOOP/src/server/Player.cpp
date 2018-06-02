#include "Player.h"
#include "../shared/Console.h"
#include "../shared/PacketHandler.h"
#include "../shared/CommandHandler.h"
#include "Server.h"
#include "commands/PlayerCommand.h"
#include "../shared/Helper.h"


Player::Player() {
}


Player::~Player() {
	disconnect();
}


void Player::start() {
	console = server->getConsole();
	if (running) {
		console->log("Player is already connected", Console::LogLevel::ERROR);
		return;
	}
	running = true;
	//Start threads
	receiveTCPThread = std::thread(&Player::threadedTCPReceive, this);
	console->log("PL_Receive: started", Console::LogLevel::INFO);
	sendTCPThread = std::thread(&Player::threadedTCPSend, this);
	console->log("PL_Send_TCP: started", Console::LogLevel::INFO);
	sendUDPThread = std::thread(&Player::threadedUDPSend, this);
	console->log("PL_Send_UDP: started", Console::LogLevel::INFO);
	console->log("Sending player current galaxy", Console::LogLevel::INFO);
	sf::Packet p;
	p << static_cast<sf::Uint8>(PacketHandler::Type::GALAXY) << server->getGalaxy();
	toSendTCP.push(p);
}


void Player::disconnect() {
	console->log("Shutting down connection", Console::LogLevel::INFO);
	running = false;
	tcpSocket->disconnect();
	console->log("PL_Socket: closed", Console::LogLevel::INFO);
	if (receiveTCPThread.joinable()) {
		receiveTCPThread.join();
		console->log("PL_Receive: joined", Console::LogLevel::INFO);
	}
	if (sendTCPThread.joinable()) {
		toSendTCP.push(sf::Packet());
		sendTCPThread.join();
		console->log("PL_Send_TCP: joined", Console::LogLevel::INFO);
	}
	if (sendUDPThread.joinable()) {
		toSendUDP.push(sf::Packet());
		sendUDPThread.join();
		console->log("PL_Send_UDP: joined", Console::LogLevel::INFO);
	}
}


void Player::sendText(std::string msg) {
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::TEXT) << msg;
	toSendTCP.push(packet);
}

void Player::sendEntity(shared_ptr<EntityCore> entity) {
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::ENTITY);
	packet << static_cast<sf::Uint8>(PacketHandler::Entity::CREATE);
	packet << *entity;
	toSendTCP.push(packet);
}

void Player::updateEntity(sf::Packet p) {
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::ENTITY);
	packet << static_cast<sf::Uint8>(PacketHandler::Entity::MODIFY);
	concatPackets(packet, p);
	toSendUDP.push(packet);
}

void Player::removeEntity(UUID id) {
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::ENTITY);
	packet << static_cast<sf::Uint8>(PacketHandler::Entity::REMOVE);
	packet << id;
	toSendTCP.push(packet);
}

void Player::removeAll() {
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::ENTITY);
	packet << static_cast<sf::Uint8>(PacketHandler::Entity::REMOVE_ALL);
	toSendTCP.push(packet);
}

void Player::warp(int destination) {
	server->warp(getptr(), destination);
}

void Player::threadedTCPReceive() {
	while (running) {
		sf::Packet packet;
		auto status = tcpSocket->receive(packet);
		switch (status) {
		case sf::TcpSocket::Disconnected:
			console->log("Client disconnected", Console::LogLevel::INFO);
			running = false;
			break;
		case sf::TcpSocket::Done:
			handlePacket(packet);
			break;
		default:
			console->log("Error receiving packet", Console::LogLevel::ERROR);
			break;
		}
	}
	console->log("Client successfully disconnected", Console::LogLevel::INFO);
	tcpSocket->disconnect();
	console->log("PL_Socket: closed", Console::LogLevel::INFO);
	server->updateConnectedList();
}


void Player::threadedTCPSend() {
	while (running) {
		sf::Packet p = toSendTCP.poll();
		if (p.getDataSize() > 0) {
			tcpSocket->send(p);
		}
	}
}

void Player::threadedUDPSend() {
	while (running) {
		sf::Packet p = toSendUDP.poll();
		if (p.getDataSize() > 0) {
			udpSocket->send(p, ip, port);
		}
	}
}

void Player::handlePacket(sf::Packet& packet) {
	sf::Uint8 type;
	if (!(packet >> type)) {
		console->log("Could not decode packet", Console::LogLevel::ERROR);
	}
	switch (type) {
	case static_cast<sf::Uint8>(PacketHandler::Type::TEXT) :
	{
		std::string msg;
		packet >> msg;
		console->log(msg, Console::LogLevel::INFO);
		break;
	}
	case static_cast<sf::Uint8>(PacketHandler::Type::COMMAND) :
		CommandID commandId;
		packet >> commandId;
		try {
			shared_ptr<Command> command = server->commandHandler.getCommand(commandId);
			if (auto p = std::dynamic_pointer_cast<PlayerCommand>(command)) {
				packet >> *p;
				p->player = this;
				p->execute();
				//console->logToConsole(p->name, Console::LogLevel::INFO);
			}
		} catch (std::exception e) {
			console->log("Player sent invalid/malformatted command (" + std::to_string(commandId) + ")", Console::LogLevel::WARNING);
		}
		break;
	}
}
