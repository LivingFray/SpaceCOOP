#include <SFML\Graphics.hpp>
#include "server\Server.h"
#include "client\Client.h"
#include "shared\Console.h"
#include "State.h"
#include "TitleScreen.h"

int main() {
	if (!sf::Shader::isAvailable()) {
		//TODO: Fancier error message
		return 1;
	}
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Spaaaaaace");
	sf::Clock clock;
	{
		auto title = std::make_shared<TitleScreen>();
		title->window = &window;
		State::setState(title);
	}
	/*
	Client client;
	Server server;
	server.port = 5000;
	server.start();
	client.ip = "127.0.0.1";
	client.port = 5000;
	client.window = &window;
	client.connect();
	*/
	clock.restart();
	window.setKeyRepeatEnabled(false);
	while (window.isOpen()) {
		auto state = State::advanceState();
		//Poll events
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
			if (e.type == sf::Event::KeyPressed) {
				//client.keyEvent(e);
				state->keyEvent(e);
			}
			if (e.type == sf::Event::KeyReleased) {
				//client.keyEvent(e);
				state->keyEvent(e);
			}
			if (e.type == sf::Event::TextEntered) {
				//client.textEvent(e);
				state->textEvent(e);
			}
			if (e.type == sf::Event::Resized) {
				//client.resizeEvent(e);
				state->resizeEvent(e);
			}
			if (e.type == sf::Event::MouseButtonPressed) {
				state->mouseEvent(e);
			}
			if (e.type == sf::Event::MouseButtonReleased) {
				state->mouseEvent(e);
			}
		}
		sf::Time t = clock.restart();
		//Update server
		//server.update(t.asSeconds());
		//Update client
		//client.update(t.asSeconds());
		state->update(t.asSeconds());
		//Draw client
		//client.draw();
		state->draw();
	}
	//server.stop();
	//client.disconnect();
	return 0;
}