#include "Player.h"
#include "../shared/Console.h"
#include "../shared/PacketHandler.h"
#include "Server.h"


Player::Player(){
}


Player::~Player() {
	if (receiveThread.joinable()) {
		receiveThread.join();
	}
}


void Player::start() {
	if (running) {
		Console::log("Player is already connected", Console::LogLevel::ERROR);
		return;
	}
	running = true;
	//Start thread
	receiveThread = std::thread(&Player::receive, this);
}


void Player::disconnect() {
	if (!running) {
		Console::log("Player is already disconnected", Console::LogLevel::ERROR);
		return;
	}
	Console::log("Shutting down connection", Console::LogLevel::INFO);
	running = false;
	socket->disconnect();
	receiveThread.join();
}


void Player::receive() {
	while (running) {
		sf::Packet packet;
		auto status = socket->receive(packet);
		switch (status) {
		case sf::TcpSocket::Disconnected:
			Console::log("Client disconnected", Console::LogLevel::INFO);
			running = false;
			break;
		case sf::TcpSocket::Done:
			handlePacket(packet);
			break;
		default:
			Console::log("Error receiving packet", Console::LogLevel::ERROR);
			break;
		}
	}
	Console::log("Client successfully disconnected", Console::LogLevel::INFO);
	socket->disconnect();
	server->updateConnectedList();
}


void Player::handlePacket(sf::Packet& packet) {
	sf::Uint8 type;
	if (!(packet >> type)) {
		Console::log("Could not decode packet", Console::LogLevel::ERROR);
	}
	switch (type) {
	case static_cast<sf::Uint8>(PacketHandler::Type::TEXT) :
		std::string msg;
		packet >> msg;
		Console::log(msg, Console::LogLevel::INFO);
		break;
	}
}
