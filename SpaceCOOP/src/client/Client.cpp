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
#include "commands/PreciseRotateClientCommand.h"
#include "commands/WarpClientCommand.h"
#include "commands/ShowConsoleCommand.h"
#include "commands/ShowSystemClientCommand.h"
#include "commands/ShowGalaxyClientCommand.h"
#include "commands/FireClientCommand.h"
#include "../shared/Command.h"
#include "../shared/EntityHandler.h"
#include "../shared/entities/EntityCore.h"
#include "../shared/entities/Ship.h"
#include "../shared/entities/Planet.h"
#include "../shared/entities/EntityStar.h"
#include "../shared/Helper.h"
#include "../shared/LaserBeam.h"

#define BINDCMD(k, c) { \
shared_ptr<ClientCommand> _cmd = std::dynamic_pointer_cast<ClientCommand>(commandHandler.getCommand(c)); \
_cmd->client = this; \
inputHandler.bind(k, _cmd); \
}


Client::Client() {
	//Register commands here
	REGCMD(ForwardsClientCommand);
	REGCMD(BackwardsClientCommand);
	REGCMD(StrafeLeftClientCommand);
	REGCMD(StrafeRightClientCommand);
	REGCMD(RotateLeftClientCommand);
	REGCMD(RotateRightClientCommand);
	REGCMD(PreciseRotateClientCommand);
	REGCMD(FireClientCommand);
	REGCMD(WarpClientCommand);
	REGCMD(ShowConsoleCommand);
	REGCMD(ShowSystemClientCommand);
	REGCMD(ShowGalaxyClientCommand);

	//Register inputs here (TODO: startup commands i.e. autoexec.cfg)
	BINDCMD(sf::Keyboard::W, "forwards");
	BINDCMD(sf::Keyboard::S, "backwards");
	BINDCMD(sf::Keyboard::A, "strafeleft");
	BINDCMD(sf::Keyboard::D, "straferight");
	BINDCMD(sf::Keyboard::Q, "rotateleft");
	BINDCMD(sf::Keyboard::E, "rotateright");
	BINDCMD(sf::Keyboard::F1, "showconsole");
	BINDCMD(sf::Keyboard::M, "systemmap");
	BINDCMD(sf::Keyboard::Tab, "togglegalaxy");
	BINDCMD(sf::Keyboard::Space, "fire");

	//Register entities here
	REGENT(Ship);
	REGENT(Planet);
	REGENT(EntityStar);

	REGPROJ(LaserBeam);
}


Client::~Client() {
	disconnect();
}

void Client::init() {
	//Set up console
	console.loadFont("assets/cour.ttf");

	hud.client = this;

	solarSystem.client = this;
	solarSystem.init();

	//Fake a resize event
	sf::Event e;
	e.type = sf::Event::Resized;
	e.size.width = 1280;
	e.size.height = 720;
	resizeEvent(e);
}

void Client::connect() {
	if (connected) {
		console.log("Already connected", GraphicalConsole::LogLevel::ERROR);
		return;
	}
	connected = true;
	receiveTCPThread = thread(&Client::threadedTCPReceive, this);
	console.log("CL_Receive_TCP: started", GraphicalConsole::LogLevel::INFO);
	receiveUDPThread = thread(&Client::threadedUDPReceive, this);
	console.log("CL_Receive_UDP: started", GraphicalConsole::LogLevel::INFO);
	sendTCPThread = thread(&Client::threadedTCPSend, this);
	console.log("CL_Send_TCP: started", GraphicalConsole::LogLevel::INFO);
	sendUDPThread = thread(&Client::threadedUDPSend, this);
	console.log("CL_Send_UDP: started", GraphicalConsole::LogLevel::INFO);
}

void Client::disconnect() {
	connected = false;
	tcpSocket.disconnect();
	console.log("CL_Socket_TCP: closed", GraphicalConsole::LogLevel::INFO);
	if (receiveTCPThread.joinable()) {
		receiveTCPThread.join();
		console.log("CL_Receive_TCP: joined", GraphicalConsole::LogLevel::INFO);
	}
	udpSocket.unbind();
	if (receiveUDPThread.joinable()) {
		receiveUDPThread.join();
		console.log("CL_Receive_UDP: joined", GraphicalConsole::LogLevel::INFO);
	}
	if (sendTCPThread.joinable()) {
		tcpToSend.push(sf::Packet());
		sendTCPThread.join();
		console.log("CL_Send_TCP: joined", GraphicalConsole::LogLevel::INFO);
	}
	if (sendUDPThread.joinable()) {
		udpToSend.push(sf::Packet());
		sendUDPThread.join();
		console.log("CL_Send_UDP: joined", GraphicalConsole::LogLevel::INFO);
	}
}

void Client::sendText(std::string msg) {
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::TEXT) << msg;
	tcpToSend.push(packet);
}

void Client::sendCommand(ClientCommand* cmd) {
	sf::Packet packet;
	packet << static_cast<sf::Uint8>(PacketHandler::Type::COMMAND) << *cmd;
	tcpToSend.push(packet);
}

void Client::draw() {
	window->clear(sf::Color::Black);
	solarSystem.draw();
	//Draw UI things here
	window->setView(uiView);
	if (galaxyMapVisible) {
		sf::RectangleShape backing(sf::Vector2f(sWidth, sHeight));
		backing.setFillColor(sf::Color::Black);
		window->draw(backing);
		window->draw(galaxy);
	}
	window->draw(hud);
	if (consoleVisible) {
		window->draw(console);
	}
	window->display();
}

void Client::update(double dt) {
	consoleJustVisible = false;
	solarSystem.update(dt);
	if (galaxyMapVisible) {
		sf::Vector2i pos = sf::Mouse::getPosition(*window);
		sf::Vector2f mapPos = window->mapPixelToCoords(pos, uiView);
		mapPos = galaxy.getInverseTransform().transformPoint(mapPos);
		galaxy.selectStar(mapPos.x, mapPos.y);
	}
	hud.update();
}

void Client::addEntity(shared_ptr<EntityCore> entity) {
	solarSystem.addEntity(entity);
}

shared_ptr<EntityCore> Client::getEntity(UUID id) {
	return solarSystem.getEntity(id);
}

void Client::removeEntity(UUID id) {
	solarSystem.removeEntity(id);
}

void Client::removeAll() {
	solarSystem.removeAll();
}

void Client::showConsole() {
	consoleVisible = true;
	//console.command.clear();
	consoleJustVisible = true;
}

void Client::setSystemMapVisibility(bool visible) {
	solarSystem.setMapVisibility(visible);
}

void Client::toggleGalaxyMapVisible() {
	galaxyMapVisible = !galaxyMapVisible;
}

shared_ptr<Ship> Client::getShip() {
	return solarSystem.ship;
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
		auto cmd = commandHandler.getCommand(cmdString.substr(0, cmdString.find_first_of(' ')));
		if (cmd) {
			auto clCommand = std::dynamic_pointer_cast<ClientCommand>(cmd);
			clCommand->client = this;
			clCommand->parseString(console.command.toAnsiString());
			console.log(console.command, GraphicalConsole::LogLevel::INFO);
		} else {
			console.log("Command not recognised: " + console.command, GraphicalConsole::LogLevel::ERROR);
		}
		console.command.clear();
	}
}

void Client::textEvent(sf::Event e) {
	if (e.text.unicode < 32) {
		return;
	}
	if (consoleVisible && !consoleJustVisible) {
		console.command += e.text.unicode;
	}
}

void Client::resizeEvent(sf::Event e) {
	sWidth = static_cast<float>(e.size.width);
	sHeight = static_cast<float>(e.size.height);
	console.resize(e.size.width, e.size.height);
	solarSystem.resizeEvent(e);
	//UI stuff
	resizeGalaxyMap();
	hud.resize(e.size.width, e.size.height);
}

void Client::mouseEvent(sf::Event e) {
	if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left && galaxyMapVisible) {
		shared_ptr<WarpClientCommand> warp = std::make_shared<WarpClientCommand>();
		warp->client = this;
		warp->destination = galaxy.selected;
		warp->execute();
	}
}

void Client::threadedTCPReceive() {
	console.log("Attempting to connect to server", GraphicalConsole::LogLevel::INFO);
	connected = tcpSocket.connect(sf::IpAddress(ip), port) == sf::TcpSocket::Done;
	console.log("CL_Socket: opened", GraphicalConsole::LogLevel::INFO);
	if (connected) {
		console.log("Connected to server at " + ip + ":" + std::to_string(port), GraphicalConsole::LogLevel::INFO);
	} else {
		console.log("Could not connect to server", GraphicalConsole::LogLevel::WARNING);
	}
	sf::Packet packet;
	while (connected) {
		sf::TcpSocket::Status st = tcpSocket.receive(packet);
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
	tcpSocket.disconnect();
	console.log("CL_Socket: closed", GraphicalConsole::LogLevel::INFO);
}

void Client::threadedTCPSend() {
	while (connected) {
		sf::Packet p = tcpToSend.poll();
		if (p.getDataSize() > 0) {
			tcpSocket.send(p);
		}
	}
}

void Client::threadedUDPReceive() {
	udpSocket.bind(tcpSocket.getLocalPort());
	console.log("Bound UDP socket on port " + std::to_string(udpSocket.getLocalPort()), GraphicalConsole::LogLevel::INFO);
	addr = sf::IpAddress(ip);
	sf::Packet packet;
	sf::IpAddress remote;
	unsigned short packetPort;
	while (connected) {
		console.log("Listening", GraphicalConsole::LogLevel::INFO);
		sf::UdpSocket::Status st = udpSocket.receive(packet, remote, packetPort);
		if (remote != addr || port != packetPort) {
			console.log("Received packet from unknown address " + remote.toString() + ":" + std::to_string(packetPort), GraphicalConsole::LogLevel::WARNING);
			console.log(std::to_string(st), GraphicalConsole::LogLevel::INFO);
			continue;
		}
		switch (st) {
			case sf::UdpSocket::Done:
				console.log("Handling new packet", GraphicalConsole::LogLevel::INFO);
				handlePacket(packet);
				console.log("Handled", GraphicalConsole::LogLevel::INFO);
				break;
			default:
				console.log("Error receiving packet", GraphicalConsole::LogLevel::ERROR);
		}
	}
	console.log("UDP channel disconnected", GraphicalConsole::LogLevel::ERROR);
}

void Client::threadedUDPSend() {
	while (connected) {
		sf::Packet p = udpToSend.poll();
		if (p.getDataSize() > 0) {
			udpSocket.send(p, addr, port);
		}
	}
}


void Client::handlePacket(sf::Packet& packet) {
	sf::Uint8 type;
	if (!(packet >> type)) {
		console.log("Could not decode packet", GraphicalConsole::LogLevel::ERROR);
	}
	console.log(std::to_string(type), GraphicalConsole::LogLevel::INFO);
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
		resizeGalaxyMap();
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
		sf::Uint8 entPacketType;
		packet >> entPacketType;
		UUID id;
		packet >> id;
		switch (entPacketType) {
		case static_cast<sf::Uint8>(PacketHandler::Entity::CREATE) : {
			EntityType entityType;
			packet >> entityType;
			try {
				shared_ptr<EntityCore> entity = entityHandler.getEntity(entityType);
				//Parse to client type?
				packet >> *entity;
				entity->id = id;
				addEntity(entity);
				console.log("Added entity of type " + std::to_string(entityType) + " and ID " + std::to_string(id), GraphicalConsole::LogLevel::INFO);
			} catch (std::exception e) {
				console.log("Received invalid entity from server ("+ std::to_string(entityType)+")", GraphicalConsole::LogLevel::WARNING);
				console.log(e.what(), GraphicalConsole::LogLevel::ERROR);
			}
			break;
		}
		case static_cast<sf::Uint8>(PacketHandler::Entity::MODIFY) : {
			auto ent = getEntity(id);
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
		case static_cast<sf::Uint8>(PacketHandler::Entity::REMOVE_ALL) : {
			removeAll();
			break;
		}
		}
		break;
	}
	case static_cast<sf::Uint8>(PacketHandler::Type::ASSIGN_SHIP) : {
		UUID id;
		packet >> id;
		auto ent = getEntity(id);
		if (!ent) {
			console.log("Received ship ID from server that does not exist " + std::to_string(id), GraphicalConsole::LogLevel::ERROR);
		}
		solarSystem.ship = std::dynamic_pointer_cast<Ship>(getEntity(id));
		if (!solarSystem.ship) {
			console.log("Received ship ID from server for non ship entity " + std::to_string(id), GraphicalConsole::LogLevel::ERROR);
		}
		break;
	}
	case static_cast<sf::Uint8>(PacketHandler::Type::PROJECTILE) : {
		console.log("projectile received (" + std::to_string(type) + ")", GraphicalConsole::LogLevel::INFO);
		sf::Uint8 projType;
		packet >> projType;
		auto proj = projectileHandler.getProjectile(projType);
		if (!proj) {
			console.log("Unknown projectile type received (" + std::to_string(projType) + ")", GraphicalConsole::LogLevel::ERROR);
			break;
		}
		packet >> *proj;
		auto ent = getEntity(proj->origin);
		if (!ent) {
			console.log("Projectile Origin not known " + std::to_string(ent->id), GraphicalConsole::LogLevel::WARNING);
		}
		//Don't add projectiles client fired, these were already predicted
		if (ent && ent->id != getShip()->id) {
			solarSystem.addProjectile(proj);
			console.log("Successfully added projectile", GraphicalConsole::LogLevel::INFO);
		}
		break;
	}
	default:
		console.log("Unknown packet type received (" + std::to_string(type) + ")", GraphicalConsole::LogLevel::ERROR);
	}
}

void Client::resizeGalaxyMap() {
	uiView = sf::View(sf::FloatRect(0.0f, 0.0f, sWidth, sHeight));
	float drawX = 0.0f;
	float drawY = 0.0f;
	float drawWidth = sWidth;
	float drawHeight = sHeight;
	float w = galaxy.maxX - galaxy.minX;
	float h = galaxy.maxY - galaxy.minY;
	float scaleX = drawWidth / w;
	float scaleY = drawHeight / h;
	float scaleBy = scaleX < scaleY ? scaleX : scaleY;
	//Give a slight border to the map to ensure nothing gets cut off or is at the very edge
	scaleBy *= 0.95f;
	galaxy.setOrigin(w / 2, h / 2);
	galaxy.setScale(scaleBy, scaleBy);
	galaxy.setPosition(drawX + drawWidth / 2, drawY + drawHeight / 2);
}
