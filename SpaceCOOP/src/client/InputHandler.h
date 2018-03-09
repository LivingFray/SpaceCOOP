#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include "../shared/Command.h"

using std::shared_ptr;
using std::unordered_map;

class InputHandler {
public:
	InputHandler();
	~InputHandler();
	void keyEvent(sf::Event e);
	void mouseEvent(sf::Event e);
	void bind(sf::Keyboard::Key key, shared_ptr<Command> cmd);
private:
	unordered_map<sf::Keyboard::Key, shared_ptr<Command>> keyBindings;
	//TODO: Mouse move bindings
	unordered_map<sf::Mouse::Button, shared_ptr<Command>> mouseBindings;
	//TODO: Joystick bindings
};

