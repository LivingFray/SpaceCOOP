#include "Client.h"
#include "GraphicalConsole.h"
#include "../shared/PacketHandler.h"
#include <memory>
#include <functional>
#include "commands/ForwardsClientCommand.h"
#include "commands/BackwardsClientCommand.h"
#include "commands/StrafeLeftClientCommand.h"
#include "commands/StrafeRightClientCommand.h"
#include "commands/RotateLeftClientCommand.h"
#include "commands/RotateRightClientCommand.h"
#include "commands/ShowConsoleCommand.h"
#include "../shared/Command.h"
#include "../shared/EntityHandler.h"
#include "../shared/entities/EntityCore.h"
#include "../shared/entities/Ship.h"
#include "../shared/Helper.h"


#define BINDCMD(k, c) { \
shared_ptr<ClientCommand> _cmd = std::dynamic_pointer_cast<ClientCommand>(commandHandler.getCommand(c)); \
_cmd->client = this; \
inputHandler.bind(k, _cmd); \
}


Client::Client() {
	//Register commands here
	REGCMD(ForwardsClientCommand, 0);
	REGCMD(BackwardsClientCommand, 1);
	REGCMD(StrafeLeftClientCommand, 2);
	REGCMD(StrafeRightClientCommand, 3);
	REGCMD(RotateLeftClientCommand, 4);
	REGCMD(RotateRightClientCommand, 5);
	REGCMD(ShowConsoleCommand, 200);

	//Register inputs here (TODO: startup commands i.e. autoexec.cfg)
	BINDCMD(sf::Keyboard::W, "forwards");
	BINDCMD(sf::Keyboard::S, "backwards");
	BINDCMD(sf::Keyboard::A, "strafeleft");
	BINDCMD(sf::Keyboard::D, "straferight");
	BINDCMD(sf::Keyboard::Q, "rotateleft");
	BINDCMD(sf::Keyboard::E, "rotateright");
	BINDCMD(sf::Keyboard::Escape, "showconsole");

	//Register entities here
	REGENT(Ship);

	//Set up console
	console.loadFont("assets/cour.ttf");
}


Client::~Client() {
	if (receiveThread.joinable()) {
		receiveThread.join();
		console.log("CL_Receive: joined", GraphicalConsole::LogLevel::INFO);
	}
	if (sendThread.joinable()) {
		toSend.push(sf::Packet());
		sendThread.join();
		console.log("CL_Send: joined", GraphicalConsole::LogLevel::INFO);
	}
}

void Client::connect() {
	if (connected) {
		console.log("Already connected", GraphicalConsole::LogLevel::ERROR);
		return;
	}
	connected = true;
	receiveThread = thread(&Client::threadedReceive, this);
	console.log("CL_Receive: started", GraphicalConsole::LogLevel::INFO);
	sendThread = thread(&Client::threadedSend, this);
	console.log("CL_Send: started", GraphicalConsole::LogLevel::INFO);
}

void Client::disconnect() {
	if (!connected) {
		console.log("Already disconnected", GraphicalConsole::LogLevel::ERROR);
		return;
	}
	connected = false;
	socket.disconnect();
	console.log("CL_Socket: closed", GraphicalConsole::LogLevel::INFO);
	receiveThread.join();
	console.log("CL_Receive: joined", GraphicalConsole::LogLevel::INFO);
	//Kind of a hack but the easiest way to interrupt the TSQueue
	toSend.push(sf::Packet());
	sendThread.join();
	console.log("CL_Send: joined", GraphicalConsole::LogLevel::INFO);
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
	if (consoleVisible) {
		window->draw(console);
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
		console.log("Could not find entity with UUID " + id, GraphicalConsole::LogLevel::WARNING);
	}
}

void Client::keyEvent(sf::Event e) {
	if (!consoleVisible) {
		inputHandler.keyEvent(e);
	} else if (e.key.code == sf::Keyboard::Escape && e.type == sf::Event::KeyPressed) {
		consoleVisible = false;
	} else if (e.key.code == sf::Keyboard::BackSpace && e.type == sf::Event::KeyPressed && console.command.getSize() > 0) {
		console.command = console.command.substring(0, console.command.getSize() - 1);
	} else if (e.key.code == sf::Keyboard::Return && e.type == sf::Event::KeyPressed && console.command.getSize() > 0) {
		std::string cmdString;
		if (console.command[0] == '-' || console.command[0] == '+') {
			cmdString = console.command.substring(1, console.command.getSize()).toAnsiString();
		} else {
			cmdString = console.command.toAnsiString();
		}
		auto cmd = commandHandler.getCommand(cmdString);
		if (cmd) {
			auto clCommand = std::dynamic_pointer_cast<ClientCommand>(cmd);
			clCommand->client = this;
			clCommand->parseString(console.command.toAnsiString());
		}
		console.command.clear();
	}
}

void Client::textEvent(sf::Event e) {
	if (e.text.unicode < 32) {
		return;
	}
	if (consoleVisible) {
		console.command += e.text.unicode;
	}
}

void Client::threadedReceive() {
	console.log("Attempting to connect to server", GraphicalConsole::LogLevel::INFO);
	connected = socket.connect(sf::IpAddress(ip), port) == sf::TcpSocket::Done;
	console.log("CL_Socket: opened", GraphicalConsole::LogLevel::INFO);
	if (connected) {
		console.log("Connected to server at " + ip + ":" + std::to_string(port), GraphicalConsole::LogLevel::INFO);
	} else {
		console.log("Could not connect to server", GraphicalConsole::LogLevel::WARNING);
	}
	sf::Packet packet;
	while (connected) {
		sf::TcpSocket::Status st = socket.receive(packet);
		switch (st) {
		case sf::TcpSocket::Disconnected:
			connected = false;
			console.log("Connection to server was terminated", GraphicalConsole::LogLevel::INFO);
			break;
		case sf::TcpSocket::Done:
			handlePacket(packet);
			break;
		default:
			console.log("Error receiving packet", GraphicalConsole::LogLevel::ERROR);
		}
	}
	//Shutdown socket
	socket.disconnect();
	console.log("CL_Socket: closed", GraphicalConsole::LogLevel::INFO);
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
		console.log("Could not decode packet", GraphicalConsole::LogLevel::ERROR);
	}
	switch (type) {
	case static_cast<sf::Uint8>(PacketHandler::Type::TEXT) : {
		std::string msg;
		packet >> msg;
		console.log(msg, GraphicalConsole::LogLevel::INFO);
		break;
	}
	case static_cast<sf::Uint8>(PacketHandler::Type::GALAXY) : {
		packet >> galaxy;
		galaxy.createVertexArray();
		console.log("Received galaxy data", GraphicalConsole::LogLevel::INFO);
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
			console.log("Received invalid command from server", GraphicalConsole::LogLevel::WARNING);
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
				//console.log("Added entity " + std::to_string(id), GraphicalConsole::LogLevel::INFO);
			} catch (std::exception e) {
				console.log("Received invalid entity from server ("+ std::to_string(entType)+")", GraphicalConsole::LogLevel::WARNING);
			}
			break;
		}
		case static_cast<sf::Uint8>(PacketHandler::Entity::MODIFY) : {
			auto ent = entities[id];
			if (!ent) {
				console.log("Cannot modify entity, does not exist (" + std::to_string(id) + ")", GraphicalConsole::LogLevel::WARNING);
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
