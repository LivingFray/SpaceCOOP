#include "Client.h"
#include "../shared/Console.h"


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
	server.disconnect();
}

void Client::threadedReceive() {
	Console::log("Attempting to connect to server", Console::LogLevel::INFO);
	connected = server.connect(sf::IpAddress(ip), port) == sf::TcpSocket::Done;
	if (connected) {
		Console::log("Connected to server at " + ip + ":" + std::to_string(port), Console::LogLevel::INFO);
	} else {
		Console::log("Could not connect to server", Console::LogLevel::WARNING);
	}
	sf::Packet packet;
	while (connected) {
		sf::TcpSocket::Status st = server.receive(packet);
		switch(st) {
		case sf::TcpSocket::Disconnected:
			connected = false;
			Console::log("Connection to server was terminated", Console::LogLevel::INFO);
			break;
		case sf::TcpSocket::Done:
			//TODO: Handle packet received
			break;
		default:
			Console::log("Error receiving packet", Console::LogLevel::ERROR);
		}
	}
	//Shutdown socket
	server.disconnect();
}
