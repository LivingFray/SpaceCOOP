#include "ClientState.h"



ClientState::ClientState() {
}


ClientState::~ClientState() {
}

void ClientState::onActivation() {
	client.window = window;
}

void ClientState::update(double dt) {
	client.update(dt);
}

void ClientState::draw() {
	client.draw();
}

void ClientState::keyEvent(sf::Event e) {
	client.keyEvent(e);
}

void ClientState::textEvent(sf::Event e) {
	client.textEvent(e);
}

void ClientState::resizeEvent(sf::Event e) {
	client.resizeEvent(e);
}

void ClientState::connect(std::string ip, unsigned short port) {
	client.ip = ip;
	client.port = port;
	client.connect();
}
