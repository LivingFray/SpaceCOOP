#include "Client.h"
#include "../shared/Console.h"
#include "../shared/PacketHandler.h"


Client::Client() : receiveThread(&Client::threadedReceive, this) {
}


Client::~Client() {
}

void Client::connect() {
	connected = true;
	receiveThread.launch();
}

void Client::disconnect() {
	connected = false;
	socket.disconnect();
}

void Client::sendText(std::string msg) {
	//TEMP USE TSQ WHEN IMPLEMENTED
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::TEXT) << msg;
	socket.send(packet);
}

void Client::threadedReceive() {
	Console::log("Attempting to connect to server", Console::LogLevel::INFO);
	connected = socket.connect(sf::IpAddress(ip), port) == sf::TcpSocket::Done;
	if (connected) {
		Console::log("Connected to server at " + ip + ":" + std::to_string(port), Console::LogLevel::INFO);
	} else {
		Console::log("Could not connect to server", Console::LogLevel::WARNING);
	}
	sf::Packet packet;
	while (connected) {
		sf::TcpSocket::Status st = socket.receive(packet);
		switch(st) {
		case sf::TcpSocket::Disconnected:
			connected = false;
			Console::log("Connection to server was terminated", Console::LogLevel::INFO);
			break;
		case sf::TcpSocket::Done:
			handlePacket(packet);
			break;
		default:
			Console::log("Error receiving packet", Console::LogLevel::ERROR);
		}
	}
	//Shutdown socket
	socket.disconnect();
}


void Client::handlePacket(sf::Packet& packet) {
	sf::Uint8 type;
	if (!(packet >> type)) {
		Console::log("Could not decode packet", Console::LogLevel::ERROR);
	}
	switch (type) {
	case static_cast<sf::Uint8>(PacketHandler::Type::TEXT):
		std::string msg;
		packet >> msg;
		Console::log(msg, Console::LogLevel::INFO);
		break;
	}
}
