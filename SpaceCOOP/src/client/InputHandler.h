#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include "../shared/Command.h"

class InputHandler {
public:
	InputHandler();
	~InputHandler();
	void keyEvent(sf::Event e);
	void mouseEvent(sf::Event e);
	void bind(sf::Keyboard::Key key, std::shared_ptr<Command> cmd);
private:
	std::unordered_map<sf::Keyboard::Key, std::shared_ptr<Command>> keyBindings;
	//TODO: Mouse move bindings
	std::unordered_map<sf::Mouse::Button, std::shared_ptr<Command>> mouseBindings;
	//TODO: Joystick bindings
};

