#include "TitleScreen.h"
#include "LocalServerState.h"
#include "ClientState.h"


TitleScreen::TitleScreen() {
	font.loadFromFile("assets/cour.ttf");
}


TitleScreen::~TitleScreen() {
}

void TitleScreen::update(double dt) {
}

void TitleScreen::draw() {
	window->clear(sf::Color::Black);
	sf::Text title("Placeholder title, press 1 for client-server, press 2 for pure client", font);
	title.setPosition(sf::Vector2f(100.0f, 100.0f));
	window->draw(title);
	window->display();
}

void TitleScreen::keyEvent(sf::Event e) {
	if (switched) {
		return;
	}
	if (e.key.code == sf::Keyboard::Num1) {
		switched = true;
		auto newState = std::make_shared<LocalServerState>();
		newState->window = window;
		newState->startServer(5000);
		newState->connect();
		setState(newState);
	} else if (e.key.code == sf::Keyboard::Num2) {
		switched = true;
		auto newState = std::make_shared<ClientState>();
		newState->window = window;
		newState->connect("127.0.0.1", 5000);
		setState(newState);
	}
}

void TitleScreen::textEvent(sf::Event e) {
}

void TitleScreen::resizeEvent(sf::Event e) {
}

void TitleScreen::mouseEvent(sf::Event e) {
}
