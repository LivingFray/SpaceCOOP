#include "Player.h"
#include "../shared/Console.h"


Player::Player() : receiveThread(&Player::receive, this){
}


Player::~Player() {
}


void Player::start() {
	if (running) {
		Console::log("Player is already connected", Console::LogLevel::ERROR);
		return;
	}
	running = true;
	receiveThread.launch();
}


void Player::disconnect() {
	running = false;
	Console::log("Shutting down connection", Console::LogLevel::INFO);
	receiveThread.wait();
	Console::log("Client successfully disconnected", Console::LogLevel::INFO);
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
			//Handle packet here
			break;
		default:
			Console::log("Error receiving packet", Console::LogLevel::ERROR);
			break;
		}
	}
	//Shutdown socket
	socket->disconnect();
	Console::log("Player disconnect complete", Console::LogLevel::INFO);
}
