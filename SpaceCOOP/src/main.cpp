#include <SFML\Graphics.hpp>
#include "server\Server.h"
#include "client\Client.h"
#include "shared\Console.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Spaaaaaace");
	Client client;
	Server server;
	server.port = 5000;
	server.start();
	client.ip = "127.0.0.1";
	client.port = 5000;
	client.connect();
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
			}
		}

		//Draw window
		window.clear();
		window.display();
	}
	server.stop();
	client.disconnect();
	system("pause");
	return 0;
}