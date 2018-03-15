#include "Client.h"
#include "../shared/Console.h"
#include "../shared/PacketHandler.h"
#include <memory>
#include <functional>
#include "commands/ForwardsClientCommand.h"
#include "commands/BackwardsClientCommand.h"
#include "commands/StrafeLeftClientCommand.h"
#include "commands/StrafeRightClientCommand.h"
#include "../shared/Command.h"
#include "../shared/EntityHandler.h"
#include "../shared/entities/EntityCore.h"
#include "../shared/entities/Ship.h"

#define REGCMD(c, i) { \
auto _cmd = std::make_shared<c>(); \
_cmd->client = this; \
commandHandler.registerCommand(_cmd, i); \
}

#define BINDCMD(k, c) { \
shared_ptr<ClientCommand> _cmd = std::dynamic_pointer_cast<ClientCommand>(commandHandler.getCommand(c)); \
_cmd->client = this; \
inputHandler.bind(k, _cmd); \
}

#define REGENT(e) { \
std::function<shared_ptr<EntityCore>()> fun = []() { return std::make_shared<e>();}; \
entityHandler.registerEntity(fun); \
}

Client::Client() {
	//Register commands here
	REGCMD(ForwardsClientCommand, 0);
	REGCMD(BackwardsClientCommand, 1);
	REGCMD(StrafeLeftClientCommand, 2);
	REGCMD(StrafeRightClientCommand, 3);

	//Register inputs here (TODO: startup commands i.e. autoexec.cfg)
	BINDCMD(sf::Keyboard::W, "forwards");
	BINDCMD(sf::Keyboard::S, "backwards");
	BINDCMD(sf::Keyboard::A, "strafeleft");
	BINDCMD(sf::Keyboard::D, "straferight");

	//Register entities here
	REGENT(Ship);
}


Client::~Client() {
	if (receiveThread.joinable()) {
		receiveThread.join();
		Console::log("CL_Receive: joined", Console::LogLevel::INFO);
	}
	if (sendThread.joinable()) {
		toSend.push(sf::Packet());
		sendThread.join();
		Console::log("CL_Send: joined", Console::LogLevel::INFO);
	}
}

void Client::connect() {
	if (connected) {
		Console::log("Already connected", Console::LogLevel::ERROR);
		return;
	}
	connected = true;
	receiveThread = thread(&Client::threadedReceive, this);
	Console::log("CL_Receive: started", Console::LogLevel::INFO);
	sendThread = thread(&Client::threadedSend, this);
	Console::log("CL_Send: started", Console::LogLevel::INFO);
}

void Client::disconnect() {
	if (!connected) {
		Console::log("Already disconnected", Console::LogLevel::ERROR);
		return;
	}
	connected = false;
	socket.disconnect();
	Console::log("CL_Socket: closed", Console::LogLevel::INFO);
	receiveThread.join();
	Console::log("CL_Receive: joined", Console::LogLevel::INFO);
	//Kind of a hack but the easiest way to interrupt the TSQueue
	toSend.push(sf::Packet());
	sendThread.join();
	Console::log("CL_Send: joined", Console::LogLevel::INFO);
}

void Client::sendText(std::string msg) {
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::TEXT) << msg;
	toSend.push(packet);
}

void Client::sendCommand(ClientCommand* cmd) {
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::COMMAND) << *cmd;
	toSend.push(packet);
}

void Client::draw() {
	window->clear(sf::Color::Black);
	window->draw(galaxy);
	for (auto ent : entities) {
		window->draw(*ent.second);
	}
	window->display();
}

void Client::update(double dt) {
	for (auto ent : entities) {
		ent.second->update(dt);
	}
}

void Client::removeEntity(UUID id) {
	if (!entities.erase(id)) {
		Console::log("Could not find entity with UUID " + id, Console::LogLevel::WARNING);
	}
}

void Client::threadedReceive() {
	Console::log("Attempting to connect to server", Console::LogLevel::INFO);
	connected = socket.connect(sf::IpAddress(ip), port) == sf::TcpSocket::Done;
	Console::log("CL_Socket: opened", Console::LogLevel::INFO);
	if (connected) {
		Console::log("Connected to server at " + ip + ":" + std::to_string(port), Console::LogLevel::INFO);
	} else {
		Console::log("Could not connect to server", Console::LogLevel::WARNING);
	}
	sf::Packet packet;
	while (connected) {
		sf::TcpSocket::Status st = socket.receive(packet);
		switch (st) {
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
	Console::log("CL_Socket: closed", Console::LogLevel::INFO);
}

void Client::threadedSend() {
	while (connected) {
		sf::Packet p = toSend.poll();
		if (p.getDataSize() > 0) {
			socket.send(p);
		}
	}
}


void Client::handlePacket(sf::Packet& packet) {
	sf::Uint8 type;
	if (!(packet >> type)) {
		Console::log("Could not decode packet", Console::LogLevel::ERROR);
	}
	switch (type) {
	case static_cast<sf::Uint8>(PacketHandler::Type::TEXT) : {
		std::string msg;
		packet >> msg;
		Console::log(msg, Console::LogLevel::INFO);
		break;
	}
	case static_cast<sf::Uint8>(PacketHandler::Type::GALAXY) : {
		packet >> galaxy;
		galaxy.createVertexArray();
		Console::log("Received galaxy data", Console::LogLevel::INFO);
		break;
	}
	case static_cast<sf::Uint8>(PacketHandler::Type::COMMAND) : {
		CommandID id;
		packet >> id;
		try {
			shared_ptr<Command> command = commandHandler.getCommand(id);
			if (auto c = std::dynamic_pointer_cast<ClientCommand>(command)) {
				packet >> *c;
				c->client = this;
				c->execute();
			}
		} catch (std::exception e) {
			Console::log("Received invalid command from server", Console::LogLevel::WARNING);
		}
		break;
	}
	case static_cast<sf::Uint8>(PacketHandler::Type::ENTITY) : {
		sf::Uint8 entType;
		packet >> entType;
		UUID id;
		packet >> id;
		switch (entType) {
		case static_cast<sf::Uint8>(PacketHandler::Entity::CREATE) : {
			try {
				shared_ptr<EntityCore> entity = entityHandler.getEntity(entType);
				//Parse to client type?
				packet >> *entity;
				entity->id = id;
				entities.insert_or_assign(id, entity);
				//Console::log("Added entity " + std::to_string(id), Console::LogLevel::INFO);
			} catch (std::exception e) {
				Console::log("Received invalid entity from server ("+ std::to_string(entType)+")", Console::LogLevel::WARNING);
			}
			break;
		}
		case static_cast<sf::Uint8>(PacketHandler::Entity::MODIFY) : {
			auto ent = entities[id];
			if (!ent) {
				Console::log("Cannot modify entity, does not exist (" + std::to_string(id) + ")", Console::LogLevel::WARNING);
				break;
			}
			ent->modify(packet);
			break;
		}
		case static_cast<sf::Uint8>(PacketHandler::Entity::REMOVE) : {
			removeEntity(id);
			break;
		}
		}
		break;
	}
	case static_cast<sf::Uint8>(PacketHandler::Type::ASSIGN_SHIP) : {
		UUID id;
		packet >> id;
		ship = std::dynamic_pointer_cast<Ship>(entities[id]);
		break;
	}
	}
}
