#include "CommandHandler.h"
#include "HeldCommand.h"

CommandHandler::CommandHandler() {
}


CommandHandler::~CommandHandler() {
}

shared_ptr<Command> CommandHandler::getCommand(CommandID id) {
	if (id >= commands.size() || id < 0) {
		throw std::exception("Command not recognised");
	}
	return commands[id];
}

shared_ptr<Command> CommandHandler::getCommand(std::string name) {
	return namedCommands[name];
}

void CommandHandler::registerCommand(shared_ptr<Command> c, CommandID id) {
	if (id >= commands.size() || id < 0) {
		throw std::exception("Command ID out of range");
	}
	if (commands[id]) {
		throw std::exception("Command already registered to ID " + id);
	}
	c->id = id;
	commands[id] = c;
	namedCommands[c->name] = c;
}
