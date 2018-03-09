#include "InputHandler.h"
#include "../shared/HeldCommand.h"
#include <memory>


InputHandler::InputHandler() {
}


InputHandler::~InputHandler() {
}

void InputHandler::keyEvent(sf::Event e) {
	//Handle +/- codes (+ = onPress - = onRelease)
	auto p = keyBindings.find(e.key.code);
	if (p != keyBindings.end()) {
		shared_ptr<Command> c = (*p).second;
		//Dynamically cast command to HeldCommand
		if (shared_ptr<HeldCommand> h = std::dynamic_pointer_cast<HeldCommand>(c)) {
			h->held = e.type == e.KeyPressed;
		}
		c->execute();
	}

}

void InputHandler::mouseEvent(sf::Event e) {
}

void InputHandler::bind(sf::Keyboard::Key key, shared_ptr<Command> cmd) {
	keyBindings.insert_or_assign(key, cmd);
}
