#include "LocalServerState.h"



LocalServerState::LocalServerState() {
}


LocalServerState::~LocalServerState() {
	client.disconnect();
	server.stop();
}

void LocalServerState::onActivation() {
	client.window = window;
	client.init();
}

void LocalServerState::update(double dt) {
	server.update(dt);
	client.update(dt);
}

void LocalServerState::draw() {
	window->clear(sf::Color::Black);
	client.draw();
}

void LocalServerState::keyEvent(sf::Event e) {
	client.keyEvent(e);
}

void LocalServerState::textEvent(sf::Event e) {
	client.textEvent(e);
}

void LocalServerState::resizeEvent(sf::Event e) {
	client.resizeEvent(e);
}

void LocalServerState::mouseEvent(sf::Event e) {
	client.mouseEvent(e);
}

void LocalServerState::startServer(unsigned short port) {
	server.port = port;
	server.start();
}

void LocalServerState::connect() {
	client.ip = "127.0.0.1";
	client.port = server.port;
	client.connect();
}
