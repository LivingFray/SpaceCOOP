#include <SFML\Graphics.hpp>
#include "server\Server.h"
#include "client\Client.h"
#include "shared\Console.h"

int main() {
	if (!sf::Shader::isAvailable()) {
		//TODO: Fancier error message
		return 1;
	}
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Spaaaaaace");
	sf::Clock clock;
	Client client;
	Server server;
	server.port = 5000;
	server.start();
	client.ip = "127.0.0.1";
	client.port = 5000;
	client.window = &window;
	client.connect();
	clock.restart();
	while (window.isOpen()) {
		//Poll events
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
			if (e.type == sf::Event::KeyPressed) {
				if (e.key.code == sf::Keyboard::Space) {
					Console::log("Severing connection from client end", Console::LogLevel::INFO);
					client.disconnect();
				}
				if (e.key.code == sf::Keyboard::Return) {
					client.sendText("This is a test");
				}
			}
		}
		sf::Time t = clock.restart();
		//Update server
		server.update(t.asSeconds());
		//Draw client
		client.draw();
	}
	server.stop();
	client.disconnect();
	return 0;
}