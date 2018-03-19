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
	if (running) {
		disconnect();
	}
}


void Player::start() {
	if (running) {
		Console::logToConsole("Player is already connected", Console::LogLevel::ERROR);
		return;
	}
	running = true;
	//Start threads
	receiveThread = std::thread(&Player::threadedReceive, this);
	Console::logToConsole("PL_Receive: started", Console::LogLevel::INFO);
	sendThread = std::thread(&Player::threadedSend, this);
	Console::logToConsole("PL_Send: started", Console::LogLevel::INFO);
	Console::logToConsole("Sending player current galaxy", Console::LogLevel::INFO);
	sf::Packet p;
	p << static_cast<sf::Uint8>(PacketHandler::Type::GALAXY) << server->getGalaxy();
	toSend.push(p);
}


void Player::disconnect() {
	if (!running) {
		Console::logToConsole("Player is already disconnected", Console::LogLevel::ERROR);
		return;
	}
	Console::logToConsole("Shutting down connection", Console::LogLevel::INFO);
	running = false;
	socket->disconnect();
	Console::logToConsole("PL_Socket: closed", Console::LogLevel::INFO);
	receiveThread.join();
	Console::logToConsole("PL_Receive: joined", Console::LogLevel::INFO);
	toSend.push(sf::Packet());
	sendThread.join();
	Console::logToConsole("PL_Send: joined", Console::LogLevel::INFO);
}


void Player::sendText(std::string msg) {
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::TEXT) << msg;
	toSend.push(packet);
}

void Player::sendEntity(shared_ptr<EntityCore> entity) {
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::ENTITY);
	packet << static_cast<sf::Uint8>(PacketHandler::Entity::CREATE);
	packet << *entity;
	toSend.push(packet);
}

void Player::updateEntity(sf::Packet p) {
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::ENTITY);
	packet << static_cast<sf::Uint8>(PacketHandler::Entity::MODIFY);
	concatPackets(packet, p);
	toSend.push(packet);
}

void Player::threadedReceive() {
	while (running) {
		sf::Packet packet;
		auto status = socket->receive(packet);
		switch (status) {
		case sf::TcpSocket::Disconnected:
			Console::logToConsole("Client disconnected", Console::LogLevel::INFO);
			running = false;
			break;
		case sf::TcpSocket::Done:
			handlePacket(packet);
			break;
		default:
			Console::logToConsole("Error receiving packet", Console::LogLevel::ERROR);
			break;
		}
	}
	Console::logToConsole("Client successfully disconnected", Console::LogLevel::INFO);
	socket->disconnect();
	Console::logToConsole("PL_Socket: closed", Console::LogLevel::INFO);
	server->updateConnectedList();
}


void Player::threadedSend() {
	while (running) {
		sf::Packet p = toSend.poll();
		if (p.getDataSize() > 0) {
			socket->send(p);
		}
	}
}


void Player::handlePacket(sf::Packet& packet) {
	sf::Uint8 type;
	if (!(packet >> type)) {
		Console::logToConsole("Could not decode packet", Console::LogLevel::ERROR);
	}
	switch (type) {
	case static_cast<sf::Uint8>(PacketHandler::Type::TEXT) :
	{
		std::string msg;
		packet >> msg;
		Console::logToConsole(msg, Console::LogLevel::INFO);
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
			}
		} catch (std::exception e) {
			Console::logToConsole("Player sent invalid command (" + std::to_string(commandId) + ")", Console::LogLevel::WARNING);
		}
		break;
	}
}
