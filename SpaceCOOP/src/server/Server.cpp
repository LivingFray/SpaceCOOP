#include "Server.h"
#include "../shared/Console.h"


Server::Server() : receiveThread(&Server::threadedReceive, this) {
}


Server::~Server() {
}


void Server::start() {
	if (running) {
		Console::log("Attempted to start server that was already running", Console::LogLevel::ERROR);
	}
	running = true;
	receiveThread.launch();
}


void Server::stop() {
	running = false;
	receiveThread.wait();
}


void Server::threadedReceive() {
	std::list<sf::TcpSocket*> clientTCPs;
	sf::TcpListener listen;
	sf::SocketSelector tcpSelector;
	tcpSelector.add(listen);
	listen.listen(port);
	Console::log("Started server", Console::LogLevel::INFO);
	while (running) {
		Console::log("Loop", Console::LogLevel::INFO);
		if (tcpSelector.wait()) {
			Console::log("Updating", Console::LogLevel::INFO);
			//Check for new clients
			if (tcpSelector.isReady(listen)) {
				//Connect new client to server
				sf::TcpSocket* newClient = new sf::TcpSocket();
				if (listen.accept(*newClient)) {
					//At max players, disconnect it
					if (numPlayers >= MAX_PLAYERS) {
						Console::log("Server full, disconnecting client", Console::LogLevel::WARNING);
						//TODO: Nice disconnect messages and such
						newClient->disconnect();
					} else {
						//Add player
						Console::log("New client connected", Console::LogLevel::INFO);
						clientTCPs.push_back(newClient);
					}
				}
			}
			//Go through clients and handle packets
			for (auto iter = clientTCPs.begin(); iter != clientTCPs.end(); iter++) {
				if (tcpSelector.isReady(**iter)) {
					sf::Packet packet;
					auto s = (*iter)->receive(packet);
					switch (s) {
					case sf::TcpSocket::Disconnected:
					{
						std::string msg = (*iter)->getRemoteAddress().toString() + ":" + \
							std::to_string((*iter)->getRemotePort()) + " disconnected";
						Console::log(msg, Console::LogLevel::INFO);
						delete *iter;
						iter = clientTCPs.erase(iter);
						break;
					}
					case sf::TcpSocket::Done:
						//TODO: Handle packet
						break;
					default:
						break;
					}
				}
			}
		}
	}
	//Shutdown sockets
	listen.close();
	for (sf::TcpSocket* cl : clientTCPs) {
		cl->disconnect();
	}
}